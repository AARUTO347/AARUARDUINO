// ============================================================
//  AARUARDUINO BH1750 Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruBH1750.h"

AaruarduinoError AaruBH1750::begin(uint8_t addr) {
    return begin(nullptr, addr);
}
AaruarduinoError AaruBH1750::begin(AaruI2CEngine* i2c, uint8_t addr) {
    _addr = addr; _i2c = i2c; _configured = true; _err = AARUARDUINO_OK;
    return _err;
}

bool AaruBH1750::read(float& lux) {
    _err = AARUARDUINO_OK;
    lux = 0;
    return false;
}
float AaruBH1750::readLux() {
    _err = AARUARDUINO_OK;
    return NAN;
}

AaruarduinoError AaruBH1750::setMode(BH1750Mode mode) {
    _mode = mode; _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruBH1750::setMTreg(uint8_t mtreg) {
    _err = AARUARDUINO_OK; return _err;
}

AaruarduinoError AaruBH1750::powerOn() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruBH1750::powerOff() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruBH1750::reset() { _err = AARUARDUINO_OK; return _err; }

bool AaruBH1750::isConfigured() const { return _configured; }
AaruarduinoError AaruBH1750::lastError() const { return _err; }
const char* AaruBH1750::errorString() const { return aaruarduinoErrorString(_err); }

AaruarduinoError AaruBH1750::writeCmd(uint8_t cmd) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    return _i2c->write8(_addr, cmd, 0);
}
