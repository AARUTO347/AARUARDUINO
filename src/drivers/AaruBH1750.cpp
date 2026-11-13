// ============================================================
//  AARUARDUINO BH1750 Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruBH1750.h"
#include <math.h>

// BH1750 commands (written as a single byte, no register address)
#define BH1750_POWER_DOWN   0x00
#define BH1750_POWER_ON     0x01
#define BH1750_RESET        0x02

AaruarduinoError AaruBH1750::begin(uint8_t addr) {
    return begin(nullptr, addr);
}

AaruarduinoError AaruBH1750::begin(AaruI2CEngine* i2c, uint8_t addr) {
    _addr = addr;
    _i2c = i2c;
    _configured = true;
    _err = AARUARDUINO_OK;

    if (!_i2c) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }
    powerOn();
    reset();
    setMode(BH1750_CONT_H_RES);
    return _err;
}

// Write a one byte command (the BH1750 has no register address).
AaruarduinoError AaruBH1750::writeCmd(uint8_t cmd) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    _err = _i2c->write8(_addr, cmd, 0);
    return _err;
}

AaruarduinoError AaruBH1750::setMode(BH1750Mode mode) {
    _mode = (int)mode;
    return writeCmd((uint8_t)mode);
}

AaruarduinoError AaruBH1750::setMTreg(uint8_t mtreg) {
    if (mtreg < 69 || mtreg > 254) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    _mtreg = mtreg;
    // High resolution modes take the MTreg as 0x40 | (mtreg >> 4).
    return writeCmd((uint8_t)(0x40 | (mtreg >> 4)));
}

AaruarduinoError AaruBH1750::powerOn() {
    return writeCmd(BH1750_POWER_ON);
}

AaruarduinoError AaruBH1750::powerOff() {
    return writeCmd(BH1750_POWER_DOWN);
}

AaruarduinoError AaruBH1750::reset() {
    return writeCmd(BH1750_RESET);
}

bool AaruBH1750::read(float& lux) {
    lux = NAN;
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return false; }
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return false; }

    uint8_t buf[2] = {0, 0};
    if (_i2c->readBytes(_addr, 0x00, buf, 2) != AARUARDUINO_OK) {
        _err = AARUARDUINO_READ_FAIL;
        return false;
    }
    uint16_t raw = (uint16_t)(((uint16_t)buf[0] << 8) | buf[1]);

    float resolution = 1.0f;
    if (_mode == BH1750_CONT_L_RES || _mode == BH1750_ONE_L_RES) {
        resolution = 4.0f;          // low resolution mode: 4 lux per count
    } else if (_mtreg >= 69 && _mtreg != 69) {
        resolution = (_mtreg / 69.0f) / 2.0f;
        if (resolution < 0.25f) resolution = 0.25f;
    }

    lux = (float)raw / resolution / 1.2f;
    _err = AARUARDUINO_OK;
    return true;
}

float AaruBH1750::readLux() {
    float lux = NAN;
    if (!read(lux)) return NAN;
    return lux;
}

bool AaruBH1750::isConfigured() const { return _configured; }
AaruarduinoError AaruBH1750::lastError() const { return _err; }
const char* AaruBH1750::errorString() const { return aaruarduinoErrorString(_err); }
