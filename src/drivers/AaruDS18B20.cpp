// ============================================================
//  AARUARDUINO DS18B20 Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruDS18B20.h"

AaruarduinoError AaruDS18B20::begin(uint8_t pin) {
    return begin(nullptr, pin);
}
AaruarduinoError AaruDS18B20::begin(AaruOneWireEngine* ow, uint8_t pin) {
    _ow = ow; _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    if (!_ow) _ow = new AaruOneWireEngine();
    _ow->begin(pin);
#endif
    return _err;
}

int AaruDS18B20::discover() { _count = 0; return _count; }
float AaruDS18B20::readTempC(int index) { return NAN; }
float AaruDS18B20::readTempC(const AaruOneWireROM* rom) { return NAN; }
float AaruDS18B20::readTempF(int index) {
    float c = readTempC(index);
    if (isnan(c)) return NAN;
    return c * 9.0f / 5.0f + 32.0f;
}

AaruarduinoError AaruDS18B20::requestTemperatures() {
    if (!_ow) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    _err = _ow->ds18b20Convert(nullptr);
    _last_convert = millis();
    return _err;
}
bool AaruDS18B20::conversionReady() {
#if defined(ARDUINO)
    return (millis() - _last_convert) >= 750;
#else
    return true;
#endif
}
float AaruDS18B20::readTempCAsync(int index) { return readTempC(index); }

AaruarduinoError AaruDS18B20::setResolution(int index, uint8_t bits) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruDS18B20::setResolution(const AaruOneWireROM* rom, uint8_t bits) {
    _err = AARUARDUINO_OK; return _err;
}

int AaruDS18B20::alarmSearch(AaruOneWireROM* roms, int max) { return 0; }
int AaruDS18B20::deviceCount() const { return _count; }
const AaruOneWireROM* AaruDS18B20::getROM(int index) const { return nullptr; }
bool AaruDS18B20::isConfigured() const { return _configured; }
AaruarduinoError AaruDS18B20::lastError() const { return _err; }
const char* AaruDS18B20::errorString() const { return aaruarduinoErrorString(_err); }
