// ============================================================
//  AARUARDUINO I2C Engine
//  MADE BY AARUTO
// ============================================================
#include "AaruI2CEngine.h"

AaruarduinoError AaruI2CEngine::begin(int sda, int scl, uint32_t speed_hz) {
    _begun = true; _speed_hz = speed_hz; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_MEGA_AVR)
    Wire.begin();
#else
    Wire.begin(sda, scl);
#endif
    Wire.setClock(speed_hz);
#endif
    return _err;
}

bool AaruI2CEngine::devicePresent(uint8_t addr) {
#if defined(ARDUINO)
    Wire.beginTransmission(addr);
    return (Wire.endTransmission() == 0);
#else
    return false;
#endif
}

int AaruI2CEngine::scan() {
#if defined(ARDUINO)
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) return addr;
    }
#endif
    return -1;
}

uint8_t AaruI2CEngine::scanCount() {
#if defined(ARDUINO)
    uint8_t count = 0;
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) count++;
    }
    return count;
#else
    return 0;
#endif
}

bool AaruI2CEngine::scanList(uint8_t* addrs, uint8_t max_addrs) {
#if defined(ARDUINO)
    uint8_t count = 0;
    for (uint8_t addr = 1; addr < 127 && count < max_addrs; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) addrs[count++] = addr;
    }
    return count > 0;
#else
    return false;
#endif
}

AaruarduinoError AaruI2CEngine::read8(uint8_t addr, uint8_t reg, uint8_t* val) {
    return readBytes(addr, reg, val, 1);
}
AaruarduinoError AaruI2CEngine::read16(uint8_t addr, uint8_t reg, uint16_t* val) {
    uint8_t buf[2];
    AaruarduinoError e = readBytes(addr, reg, buf, 2);
    if (e == AARUARDUINO_OK) *val = (buf[0] << 8) | buf[1];
    return e;
}
AaruarduinoError AaruI2CEngine::read24(uint8_t addr, uint8_t reg, uint32_t* val) {
    uint8_t buf[3];
    AaruarduinoError e = readBytes(addr, reg, buf, 3);
    if (e == AARUARDUINO_OK) *val = ((uint32_t)buf[0] << 16) | ((uint32_t)buf[1] << 8) | buf[2];
    return e;
}
AaruarduinoError AaruI2CEngine::read32(uint8_t addr, uint8_t reg, uint32_t* val) {
    uint8_t buf[4];
    AaruarduinoError e = readBytes(addr, reg, buf, 4);
    if (e == AARUARDUINO_OK) *val = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                                     ((uint32_t)buf[2] << 8) | buf[3];
    return e;
}
AaruarduinoError AaruI2CEngine::readBytes(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len) {
    _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    Wire.beginTransmission(addr);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) { _err = AARUARDUINO_WRITE_FAIL; return _err; }
    Wire.requestFrom(addr, len);
    for (uint8_t i = 0; i < len && Wire.available(); i++) buf[i] = Wire.read();
#endif
    return _err;
}

AaruarduinoError AaruI2CEngine::write8(uint8_t addr, uint8_t reg, uint8_t val) {
    return writeBytes(addr, reg, &val, 1);
}
AaruarduinoError AaruI2CEngine::write16(uint8_t addr, uint8_t reg, uint16_t val) {
    uint8_t buf[2] = {(uint8_t)(val >> 8), (uint8_t)val};
    return writeBytes(addr, reg, buf, 2);
}
AaruarduinoError AaruI2CEngine::writeBytes(uint8_t addr, uint8_t reg, const uint8_t* buf, uint8_t len) {
    _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(buf, len);
    if (Wire.endTransmission() != 0) { _err = AARUARDUINO_WRITE_FAIL; return _err; }
#endif
    return _err;
}

AaruarduinoError AaruI2CEngine::setBit(uint8_t addr, uint8_t reg, uint8_t bit, bool value) {
    uint8_t cur;
    AaruarduinoError e = read8(addr, reg, &cur);
    if (e != AARUARDUINO_OK) return e;
    if (value) cur |= (1 << bit); else cur &= ~(1 << bit);
    return write8(addr, reg, cur);
}
AaruarduinoError AaruI2CEngine::readBit(uint8_t addr, uint8_t reg, uint8_t bit, bool* value) {
    uint8_t cur;
    AaruarduinoError e = read8(addr, reg, &cur);
    if (e != AARUARDUINO_OK) return e;
    *value = (cur >> bit) & 1;
    return AARUARDUINO_OK;
}

bool AaruI2CEngine::probeDevice(const AaruI2CDeviceDesc* desc) {
    return devicePresent(desc->addr);
}
AaruarduinoError AaruI2CEngine::initDevice(const AaruI2CDeviceDesc* desc) {
    if (desc->init_fn) return desc->init_fn(desc->addr) ? AARUARDUINO_OK : AARUARDUINO_INIT_FAIL;
    return AARUARDUINO_OK;
}
AaruarduinoError AaruI2CEngine::readDevice(const AaruI2CDeviceDesc* desc, float* values, int max_vals) {
    if (desc->read_fn) return desc->read_fn(desc->addr, values, max_vals) ? AARUARDUINO_OK : AARUARDUINO_READ_FAIL;
    return AARUARDUINO_OK;
}

AaruarduinoError AaruI2CEngine::autoDetectDisplay(uint8_t* found_addr, uint8_t* display_type) {
    _err = AARUARDUINO_NOT_FOUND;
    return _err;
}

AaruarduinoError AaruI2CEngine::doRead(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len) {
    return readBytes(addr, reg, buf, len);
}
AaruarduinoError AaruI2CEngine::doWrite(uint8_t addr, uint8_t reg, const uint8_t* buf, uint8_t len) {
    return writeBytes(addr, reg, buf, len);
}
