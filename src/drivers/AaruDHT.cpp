// ============================================================
//  AARUARDUINO DHT Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruDHT.h"

AaruarduinoError AaruDHT::begin(uint8_t pin, AaruDHTType type) {
    _pin = pin; _type = type; _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(pin, INPUT_PULLUP);
#endif
    return _err;
}

void AaruDHT::setMinIntervalMs(unsigned long ms) { _min_interval = ms; }

bool AaruDHT::read(float& temp_c, float& hum_pct) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return false; }
#if defined(ARDUINO)
    uint8_t data[5] = {0};
    if (!readRaw(data)) { _err = AARUARDUINO_READ_FAIL; return false; }
    _hum_pct = parseHum(data);
    _temp_c = parseTemp(data);
    _last_read = millis();
    _fresh = true;
    _err = AARUARDUINO_OK;
    temp_c = _temp_c;
    hum_pct = _hum_pct;
    return true;
#else
    _err = AARUARDUINO_UNSUPPORTED; return false;
#endif
}

float AaruDHT::readTemperature() {
    float t, h;
    if (!read(t, h)) return NAN;
    return t;
}
float AaruDHT::readHumidity() {
    float t, h;
    if (!read(t, h)) return NAN;
    return h;
}
float AaruDHT::readTemperatureF() {
    float t = readTemperature();
    if (isnan(t)) return NAN;
    return t * 9.0f / 5.0f + 32.0f;
}

bool AaruDHT::fresh() const { return _fresh; }
unsigned long AaruDHT::ageMs() const {
#if defined(ARDUINO)
    return millis() - _last_read;
#else
    return 0;
#endif
}

uint8_t AaruDHT::pin() const { return _pin; }
AaruDHTType AaruDHT::type() const { return _type; }
bool AaruDHT::isConfigured() const { return _configured; }
AaruarduinoError AaruDHT::lastError() const { return _err; }
const char* AaruDHT::errorString() const { return aaruarduinoErrorString(_err); }

bool AaruDHT::readRaw(uint8_t* data) {
#if defined(ARDUINO)
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delay(1);
    pinMode(_pin, INPUT_PULLUP);
    delayMicroseconds(40);
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    for (i = 0; i < 5; i++) data[i] = 0;
    for (i = 0; i < 85; i++) {
        counter = 0;
        while (digitalRead(_pin) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) break;
        }
        laststate = digitalRead(_pin);
        if (counter == 255) break;
        if (i >= 4 && i % 2 == 0) {
            data[j / 8] <<= 1;
            if (counter > 16) data[j / 8] |= 1;
            j++;
        }
    }
    if (j >= 40 && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        return true;
    }
#endif
    return false;
}

float AaruDHT::parseTemp(const uint8_t* data) {
#if defined(ARDUINO)
    if (_type == AARU_DHT11) return (float)data[2];
    return (float)(((data[2] & 0x7F) << 8) | data[3]) / 10.0f;
#else
    return NAN;
#endif
}

float AaruDHT::parseHum(const uint8_t* data) {
#if defined(ARDUINO)
    if (_type == AARU_DHT11) return (float)data[0];
    return (float)((data[0] << 8) | data[1]) / 10.0f;
#else
    return NAN;
#endif
}
