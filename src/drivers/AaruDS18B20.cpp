// ============================================================
//  AARUARDUINO DS18B20 Driver
//  MADE BY AARUTO
//
//  Thin wrapper around the 1-Wire engine: ROM discovery, temperature read,
//  and resolution control. All ROM data is CRC checked by the engine.
// ============================================================
#include "AaruDS18B20.h"

AaruarduinoError AaruDS18B20::begin(uint8_t pin) {
    return begin(nullptr, pin);
}

AaruarduinoError AaruDS18B20::begin(AaruOneWireEngine* ow, uint8_t pin) {
    _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    if (ow) {
        _ow = ow;
        _own_ow = false;
    } else {
        if (!_ow) _ow = new AaruOneWireEngine();
        _own_ow = true;
    }
    _ow->begin(pin);
#else
    _ow = ow;
    _own_ow = false;
    (void)pin;
#endif
    _configured = true;
    _count = 0;
    _last_convert = 0;
    return _err;
}

// Discover all DS18B20 devices on the bus.
int AaruDS18B20::discover() {
    if (!_configured || !_ow) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        _count = 0;
        return -1;
    }
    _count = 0;
    int found = _ow->discover(_roms, 16);
    if (found < 0) {
        _err = _ow->lastError();
        return -1;
    }
    _count = found;
    _err = AARUARDUINO_OK;
    return _count;
}

float AaruDS18B20::readTempC(const AaruOneWireROM* rom) {
    if (!_configured || !_ow) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return NAN;
    }
    // Start a conversion and wait for it to finish (default 12 bit, 750 ms).
    _err = _ow->ds18b20Convert(rom);
    if (_err != AARUARDUINO_OK) return NAN;
#if defined(ARDUINO)
    delay(750);
#endif
    return _ow->ds18b20ReadTempC(rom);
}

float AaruDS18B20::readTempC(int index) {
    if (!_configured || !_ow) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return NAN;
    }
    if (index < 0 || index >= _count) {
        _err = AARUARDUINO_INVALID_PIN;
        return NAN;
    }
    float t = readTempC(&_roms[index]);
    _err = _ow->lastError();
    return t;
}

float AaruDS18B20::readTempF(int index) {
    float c = readTempC(index);
    if (isnan(c)) return NAN;
    return c * 9.0f / 5.0f + 32.0f;
}

AaruarduinoError AaruDS18B20::requestTemperatures() {
    if (!_configured || !_ow) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    _err = _ow->ds18b20Convert(nullptr);
#if defined(ARDUINO)
    _last_convert = millis();
#else
    _last_convert = 0;
#endif
    return _err;
}

bool AaruDS18B20::conversionReady() {
#if defined(ARDUINO)
    return (millis() - _last_convert) >= 750;
#else
    return true;
#endif
}

float AaruDS18B20::readTempCAsync(int index) {
    return readTempC(index);
}

AaruarduinoError AaruDS18B20::setResolution(int index, uint8_t bits) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (index < 0 || index >= _count) { _err = AARUARDUINO_INVALID_PIN; return _err; }
    if (bits < 9 || bits > 12) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    return _ow->ds18b20SetResolution(&_roms[index], bits);
}

AaruarduinoError AaruDS18B20::setResolution(const AaruOneWireROM* rom, uint8_t bits) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (bits < 9 || bits > 12) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    return _ow->ds18b20SetResolution(rom, bits);
}

int AaruDS18B20::alarmSearch(AaruOneWireROM* roms, int max) {
    if (!_configured || !_ow) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return -1;
    }
    if (!roms || max <= 0) { _err = AARUARDUINO_INVALID_CONFIG; return -1; }
    AaruOneWireSearchState state;
    state.last_discrepancy = 0;
    state.last_family_discrepancy = 0;
    state.last_device_flag = false;
    // The DS18B20 alarm search uses the 1-wire search with the alarm command.
    int found = 0;
    while (found < max) {
        AaruOneWireROM rom;
        if (_ow->searchROM(&rom, &state) != AARUARDUINO_OK) break;
        roms[found++] = rom;
        if (state.last_discrepancy == 0 && state.last_device_flag) break;
    }
    _err = found > 0 ? AARUARDUINO_OK : AARUARDUINO_NOT_FOUND;
    return found > 0 ? found : -1;
}

int AaruDS18B20::deviceCount() const { return _count; }

const AaruOneWireROM* AaruDS18B20::getROM(int index) const {
    if (index < 0 || index >= _count) return nullptr;
    return &_roms[index];
}

bool AaruDS18B20::isConfigured() const { return _configured; }
AaruarduinoError AaruDS18B20::lastError() const { return _err; }
const char* AaruDS18B20::errorString() const { return aaruarduinoErrorString(_err); }
