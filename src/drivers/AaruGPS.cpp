// ============================================================
//  AARUARDUINO GPS Driver (NMEA 0183)
//  MADE BY AARUTO
// ============================================================
#include "AaruGPS.h"
#include <string.h>

// Portable millisecond timestamp (host builds have no clock).
static uint32_t aaruGpsNow() {
#if defined(ARDUINO)
    return aaruGpsNow();
#else
    return 0;
#endif
}

// ------------------------------------------------------------
//  Field helpers
// ------------------------------------------------------------

// Lightweight float parser. Deliberately avoids atof()/strtod(): those work in
// double precision and pull several hundred bytes of AVR double-math into
// every sketch that links this driver.
static float aaruParseFloat(const char* s) {
    if (!s) return 0.0f;
    while (*s == ' ' || *s == '\t') s++;
    bool neg = false;
    if (*s == '-') { neg = true; s++; }
    else if (*s == '+') { s++; }

    float value = 0.0f;
    while (*s >= '0' && *s <= '9') {
        value = (value * 10.0f) + (float)(*s - '0');
        s++;
    }
    if (*s == '.') {
        s++;
        float scale = 0.1f;
        while (*s >= '0' && *s <= '9') {
            value += (float)(*s - '0') * scale;
            scale *= 0.1f;
            s++;
        }
    }
    return neg ? -value : value;
}

// Lightweight signed integer parser (avoids atol()/strtol()).
static long aaruParseLong(const char* s) {
    if (!s) return 0;
    while (*s == ' ' || *s == '\t') s++;
    bool neg = false;
    if (*s == '-') { neg = true; s++; }
    else if (*s == '+') { s++; }

    long v = 0;
    while (*s >= '0' && *s <= '9') {
        v = (v * 10) + (*s - '0');
        s++;
    }
    return neg ? -v : v;
}

// Parse two hex digits into a byte (0-255).
static uint8_t aaruParseHex2(const char* s) {
    if (!s) return 0;
    uint8_t v = 0;
    for (int i = 0; i < 2 && s[i]; i++) {
        char c = s[i];
        uint8_t d;
        if (c >= '0' && c <= '9') d = (uint8_t)(c - '0');
        else if (c >= 'A' && c <= 'F') d = (uint8_t)(c - 'A' + 10);
        else if (c >= 'a' && c <= 'f') d = (uint8_t)(c - 'a' + 10);
        else break;
        v = (uint8_t)((v << 4) | d);
    }
    return v;
}

// Copy field `idx` (comma separated) of an NMEA sentence into `out`.
// Returns false when the field does not exist.
static bool nmeaField(const char* s, int idx, char* out, size_t outlen) {
    if (!s || !out || outlen == 0) return false;
    const char* p = s;
    for (int i = 0; i < idx; i++) {
        p = strchr(p, ',');
        if (!p) return false;
        p++;
    }
    size_t i = 0;
    while (*p && *p != ',' && *p != '*' && *p != '\r' && *p != '\n') {
        if (i < outlen - 1) out[i++] = *p;
        p++;
    }
    out[i] = '\0';
    return true;
}

static float nmeaFloat(const char* s, int idx, bool* ok = nullptr) {
    char buf[24];
    if (!nmeaField(s, idx, buf, sizeof(buf))) {
        if (ok) *ok = false;
        return 0.0f;
    }
    if (buf[0] == '\0') {
        if (ok) *ok = false;
        return 0.0f;
    }
    if (ok) *ok = true;
    return aaruParseFloat(buf);
}

static long nmeaLong(const char* s, int idx, bool* ok = nullptr) {
    char buf[24];
    if (!nmeaField(s, idx, buf, sizeof(buf))) {
        if (ok) *ok = false;
        return 0;
    }
    if (buf[0] == '\0') {
        if (ok) *ok = false;
        return 0;
    }
    if (ok) *ok = true;
    return aaruParseLong(buf);
}

// Convert NMEA ddmm.mmmm / dddmm.mmmm to signed decimal degrees.
static double nmeaDegrees(const char* value, char hemi) {
    if (!value || value[0] == '\0') return 0.0;
    double raw = (double)aaruParseFloat(value);
    double deg = (double)((long)(raw / 100.0));
    double min = raw - (deg * 100.0);
    double v = deg + (min / 60.0);
    if (hemi == 'S' || hemi == 'W') v = -v;
    return v;
}

// ------------------------------------------------------------
//  Lifecycle
// ------------------------------------------------------------

AaruarduinoError AaruGPS::begin(AaruUARTEngine* uart) {
    if (!uart) {
        _err = AARUARDUINO_INVALID_CONFIG;
        _configured = false;
        return _err;
    }
    _uart = uart;
    _own_uart = false;
    _configured = true;
    _buf_idx = 0;
    _in_sentence = false;
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruGPS::begin(int rx, int tx, uint32_t baud) {
    _own_uart = true;
#if defined(ESP32)
    _own_uart_engine.beginHardware(1, baud, rx, tx);
#else
    _own_uart_engine.beginSoftware(rx, tx, baud);
#endif
    return begin(&_own_uart_engine);
}

// Pull bytes from the UART and dispatch complete NMEA sentences.
void AaruGPS::update() {
    if (!_configured || !_uart) return;

    while (_uart->available() > 0) {
        int c = _uart->read();
        if (c < 0) break;

        if (c == '$') {
            _buf_idx = 0;
            _in_sentence = true;
            continue;
        }
        if (!_in_sentence) continue;
        if (c == '\r') continue;

        if (c == '\n') {
            _buf[_buf_idx] = '\0';
            _in_sentence = false;
            _buf_idx = 0;
            parseSentence(_buf);
            continue;
        }

        if (_buf_idx < (int)sizeof(_buf) - 1) {
            _buf[_buf_idx++] = (char)c;
        } else {
            // Sentence too long for the buffer - drop it and resync.
            _in_sentence = false;
            _buf_idx = 0;
        }
    }
}

// Feed a single raw NMEA sentence into the parser.
bool AaruGPS::feedSentence(const char* sentence) {
    if (!sentence || sentence[0] != '$') return false;
    if (!nmeaChecksumValid(sentence)) {
        _err = AARUARDUINO_CRC_ERROR;
        return false;
    }
    _err = AARUARDUINO_OK;

    if (strncmp(sentence, "$GP", 3) == 0 ||
        strncmp(sentence, "$GN", 3) == 0 ||
        strncmp(sentence, "$GL", 3) == 0 ||
        strncmp(sentence, "$BD", 3) == 0) {
        if (strstr(sentence, "GGA")) return parseGGA(sentence);
        if (strstr(sentence, "RMC")) return parseRMC(sentence);
        if (strstr(sentence, "GSV")) return parseGSV(sentence);
        if (strstr(sentence, "VTG")) return parseVTG(sentence);
    }
    return false;
}

// ------------------------------------------------------------
//  Accessors
// ------------------------------------------------------------

const struct AaruGPSFix& AaruGPS::fix() const { return _fix; }
bool AaruGPS::hasFix() const { return _fix.valid; }
float AaruGPS::lat() const { return _fix.latitude; }
float AaruGPS::lon() const { return _fix.longitude; }
float AaruGPS::alt() const { return _fix.altitude; }
float AaruGPS::speedKnots() const { return _fix.speed; }
float AaruGPS::speedKmph() const { return _fix.speed * 1.852f; }
float AaruGPS::course() const { return _fix.course; }
uint8_t AaruGPS::sats() const { return _fix.satellites; }
uint32_t AaruGPS::date() const { return _fix.date; }
uint32_t AaruGPS::time() const { return _fix.time; }
int AaruGPS::satCount() const { return _sat_count; }

const struct AaruGPSSatellite* AaruGPS::sat(int idx) const {
    if (idx >= 0 && idx < _sat_count) return &_sats[idx];
    return nullptr;
}

void AaruGPS::onGGA(void (*cb)(const struct AaruGPSFix&)) { _cb_gga = cb; }
void AaruGPS::onRMC(void (*cb)(const struct AaruGPSFix&)) { _cb_rmc = cb; }
void AaruGPS::onGSV(void (*cb)(const struct AaruGPSSatellite*, int)) { _cb_gsv = cb; }
void AaruGPS::onRaw(void (*cb)(const char*)) { _cb_raw = cb; }

AaruarduinoError AaruGPS::setRateHz(uint8_t hz) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruGPS::setBaud(uint32_t baud) {
    _baud = baud;
    _err = AARUARDUINO_OK;
    return _err;
}
AaruarduinoError AaruGPS::enableNMEA(uint8_t mask) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruGPS::sleep() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruGPS::wake() { _err = AARUARDUINO_OK; return _err; }

bool AaruGPS::isConfigured() const { return _configured; }
AaruarduinoError AaruGPS::lastError() const { return _err; }
const char* AaruGPS::errorString() const { return aaruarduinoErrorString(_err); }

// ------------------------------------------------------------
//  NMEA parsing
// ------------------------------------------------------------

// XOR of every character between '$' and '*' (exclusive).
uint8_t AaruGPS::nmeaChecksum(const char* s) {
    if (!s) return 0;
    uint8_t sum = 0;
    if (*s == '$') s++;
    while (*s && *s != '*') {
        sum ^= (uint8_t)(*s);
        s++;
    }
    return sum;
}

// True when the trailing *HH checksum matches the payload.
bool AaruGPS::nmeaChecksumValid(const char* s) {
    if (!s || *s != '$') return false;
    const char* star = strchr(s, '*');
    if (!star) return false;
    if (strlen(star) < 3) return false;
    char hex[3] = {star[1], star[2], '\0'};
    uint8_t given = aaruParseHex2(hex);
    return given == nmeaChecksum(s);
}

void AaruGPS::parseSentence(const char* sentence) {
    if (!sentence || sentence[0] != '$') return;

    // Raw callback receives every sentence, checksummed or not.
    if (_cb_raw) _cb_raw(sentence);

    if (!nmeaChecksumValid(sentence)) {
        _err = AARUARDUINO_CRC_ERROR;
        return;
    }
    _err = AARUARDUINO_OK;

    // Only handle the standard talker ids (GPS / GLONASS / Galileo / BeiDou).
    if (strncmp(sentence, "$GP", 3) != 0 &&
        strncmp(sentence, "$GN", 3) != 0 &&
        strncmp(sentence, "$GL", 3) != 0 &&
        strncmp(sentence, "$BD", 3) != 0) {
        return;
    }

    if (strstr(sentence, "GGA")) {
        if (parseGGA(sentence) && _cb_gga) _cb_gga(_fix);
    } else if (strstr(sentence, "RMC")) {
        if (parseRMC(sentence) && _cb_rmc) _cb_rmc(_fix);
    } else if (strstr(sentence, "GSV")) {
        if (parseGSV(sentence) && _cb_gsv) _cb_gsv(_sats, _sat_count);
    } else if (strstr(sentence, "VTG")) {
        parseVTG(sentence);
    }
}

// $..GGA,time,lat,NS,lon,EW,quality,sats,hdop,alt,M,geoid,M,age,station*hh
bool AaruGPS::parseGGA(const char* s) {
    char buf[24];
    bool ok = false;

    // UTC time hhmmss.ss -> HHMMSS
    if (nmeaField(s, 1, buf, sizeof(buf)) && buf[0]) {
        long t = aaruParseLong(buf);
        uint32_t hh = (uint32_t)(t / 10000);
        uint32_t mm = (uint32_t)((t / 100) % 100);
        uint32_t ss = (uint32_t)(t % 100);
        _fix.time = hh * 10000UL + mm * 100UL + ss;
    }

    // Latitude / longitude
    char latv[24], lonv[24], nsh[4], ewh[4];
    if (!nmeaField(s, 2, latv, sizeof(latv))) return false;
    if (!nmeaField(s, 3, nsh, sizeof(nsh))) return false;
    if (!nmeaField(s, 4, lonv, sizeof(lonv))) return false;
    if (!nmeaField(s, 5, ewh, sizeof(ewh))) return false;

    if (latv[0] && lonv[0]) {
        _fix.latitude = (float)nmeaDegrees(latv, nsh[0]);
        _fix.longitude = (float)nmeaDegrees(lonv, ewh[0]);
    }

    // Fix quality: 0 = invalid, 1 = GPS, 2 = DGPS
    long quality = nmeaLong(s, 6, &ok);
    _fix.fix_quality = ok ? (uint8_t)quality : 0;

    long nsat = nmeaLong(s, 7, &ok);
    _fix.satellites = ok ? (uint8_t)nsat : 0;

    float hdop = nmeaFloat(s, 8, &ok);
    if (ok) _fix.hdop = hdop;

    float alt = nmeaFloat(s, 9, &ok);
    if (ok) _fix.altitude = alt;

    _fix.valid = (_fix.fix_quality > 0);
    _fix.updated = aaruGpsNow();
    return true;
}

// $..RMC,time,status,lat,NS,lon,EW,speed,track,date,mag,magEW,mode*hh
bool AaruGPS::parseRMC(const char* s) {
    char buf[24];
    bool ok = false;

    if (nmeaField(s, 1, buf, sizeof(buf)) && buf[0]) {
        long t = aaruParseLong(buf);
        uint32_t hh = (uint32_t)(t / 10000);
        uint32_t mm = (uint32_t)((t / 100) % 100);
        uint32_t ss = (uint32_t)(t % 100);
        _fix.time = hh * 10000UL + mm * 100UL + ss;
    }

    // Status: A = valid, V = warning
    char status[4];
    if (!nmeaField(s, 2, status, sizeof(status))) return false;
    bool valid = (status[0] == 'A');

    char latv[24], lonv[24], nsh[4], ewh[4];
    if (nmeaField(s, 3, latv, sizeof(latv)) &&
        nmeaField(s, 4, nsh, sizeof(nsh)) &&
        nmeaField(s, 5, lonv, sizeof(lonv)) &&
        nmeaField(s, 6, ewh, sizeof(ewh))) {
        if (latv[0] && lonv[0]) {
            _fix.latitude = (float)nmeaDegrees(latv, nsh[0]);
            _fix.longitude = (float)nmeaDegrees(lonv, ewh[0]);
        }
    }

    float knots = nmeaFloat(s, 7, &ok);
    if (ok) _fix.speed = knots;

    float track = nmeaFloat(s, 8, &ok);
    if (ok) _fix.course = track;

    // Date ddmmyy -> YYYYMMDD
    if (nmeaField(s, 9, buf, sizeof(buf)) && buf[0]) {
        long d = aaruParseLong(buf);
        uint32_t day = (uint32_t)(d / 10000);
        uint32_t mon = (uint32_t)((d / 100) % 100);
        uint32_t yr = (uint32_t)(d % 100);
        uint32_t year = (yr < 80) ? (2000UL + yr) : (1900UL + yr);
        _fix.date = year * 10000UL + mon * 100UL + day;
    }

    // RMC status 'A' means the receiver has a valid fix. GGA quality is
    // reported separately, so do not require it here.
    _fix.valid = valid;
    _fix.updated = aaruGpsNow();
    return true;
}

// $..GSV,total,used,prn,elev,azimuth,snr,...*hh
bool AaruGPS::parseGSV(const char* s) {
    bool ok = false;
    long total = nmeaLong(s, 1, &ok);
    if (!ok) return false;

    _sat_count = 0;
    int idx = 3;
    while (idx + 3 < 100) {
        char prn_b[12], el_b[12], az_b[12], snr_b[12];
        if (!nmeaField(s, idx, prn_b, sizeof(prn_b))) break;
        if (prn_b[0] == '\0') break;
        if (!nmeaField(s, idx + 1, el_b, sizeof(el_b))) break;
        if (!nmeaField(s, idx + 2, az_b, sizeof(az_b))) break;
        if (!nmeaField(s, idx + 3, snr_b, sizeof(snr_b))) break;

        if (_sat_count < (int)(sizeof(_sats) / sizeof(_sats[0]))) {
            struct AaruGPSSatellite& sat = _sats[_sat_count];
            sat.prn = (uint8_t)(uint8_t)aaruParseLong(prn_b);
            sat.elevation = (uint8_t)(uint8_t)aaruParseLong(el_b);
            sat.azimuth = (uint16_t)(uint16_t)aaruParseLong(az_b);
            sat.snr = (uint8_t)(uint8_t)aaruParseLong(snr_b);
            sat.used = (snr_b[0] != '\0' && (uint8_t)aaruParseLong(snr_b) > 0);
            _sat_count++;
        }
        idx += 4;
    }

    // Satellites in view may exceed the table; report the real total.
    if (total > 0 && _fix.satellites == 0) {
        _fix.satellites = (uint8_t)(total > 255 ? 255 : total);
    }
    _fix.updated = aaruGpsNow();
    return true;
}

// $..VTG,trueTrack,T,magTrack,M,speedKnots,N,speedKph,K,mode*hh
bool AaruGPS::parseVTG(const char* s) {
    bool ok = false;

    float track = nmeaFloat(s, 1, &ok);
    if (ok) _fix.course = track;

    float knots = nmeaFloat(s, 5, &ok);
    if (ok) _fix.speed = knots;

    _fix.updated = aaruGpsNow();
    return true;
}
