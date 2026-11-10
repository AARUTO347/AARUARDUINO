// ============================================================
//  AARUARDUINO GPS Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruGPS.h"

AaruarduinoError AaruGPS::begin(AaruUARTEngine* uart) {
    _uart = uart; _configured = true; _err = AARUARDUINO_OK;
    return _err;
}
AaruarduinoError AaruGPS::begin(int rx, int tx, uint32_t baud) {
    _configured = true; _err = AARUARDUINO_OK;
    return _err;
}

void AaruGPS::update() {}

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
    if (idx < _sat_count) return &_sats[idx];
    return nullptr;
}

void AaruGPS::onGGA(void (*cb)(const AaruGPSFix&)) { _cb_gga = cb; }
void AaruGPS::onRMC(void (*cb)(const AaruGPSFix&)) { _cb_rmc = cb; }
void AaruGPS::onGSV(void (*cb)(const AaruGPSSatellite*, int)) { _cb_gsv = cb; }
void AaruGPS::onRaw(void (*cb)(const char*)) { _cb_raw = cb; }

AaruarduinoError AaruGPS::setRateHz(uint8_t hz) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruGPS::setBaud(uint32_t baud) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruGPS::enableNMEA(uint8_t mask) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruGPS::sleep() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruGPS::wake() { _err = AARUARDUINO_OK; return _err; }

bool AaruGPS::isConfigured() const { return _configured; }
AaruarduinoError AaruGPS::lastError() const { return _err; }
const char* AaruGPS::errorString() const { return aaruarduinoErrorString(_err); }

void AaruGPS::parseSentence(const char* sentence) {}
bool AaruGPS::parseGGA(const char* s) { return false; }
bool AaruGPS::parseRMC(const char* s) { return false; }
bool AaruGPS::parseGSV(const char* s) { return false; }
bool AaruGPS::parseVTG(const char* s) { return false; }
uint8_t AaruGPS::nmeaChecksum(const char* s) { return 0; }
