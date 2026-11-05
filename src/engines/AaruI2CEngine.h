// ============================================================
//  AARUARDUINO I2C Engine
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_I2C_ENGINE_H
#define AARUARDUINO_I2C_ENGINE_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"

#if defined(ARDUINO)
#include <Arduino.h>
#include <Wire.h>
#else
#include <stdint.h>
#endif

struct AaruI2CRegister {
    uint8_t reg;
    uint8_t len;
    bool readable;
    bool writable;
    float scale;
    float offset;
    const char* name;
    const char* unit;
};

struct AaruI2CDeviceDesc {
    uint8_t addr;
    uint8_t addr_alt;
    const char* name;
    const struct AaruI2CRegister* registers;
    uint8_t num_registers;
    uint8_t who_am_i_reg;
    uint8_t who_am_i_val;
    int category;
    bool (*init_fn)(uint8_t addr);
    bool (*read_fn)(uint8_t addr, float* values, int max_vals);
};

class AaruI2CEngine {
public:
    AaruI2CEngine() = default;

    AaruarduinoError begin(int sda = -1, int scl = -1, uint32_t speed_hz = 100000);
    void setTimeout(uint32_t timeout_ms);
    void setSpeed(uint32_t hz);

    bool devicePresent(uint8_t addr);
    int scan();
    uint8_t scanCount();
    bool scanList(uint8_t* addrs, uint8_t max_addrs);

    AaruarduinoError read8(uint8_t addr, uint8_t reg, uint8_t* val);
    AaruarduinoError read16(uint8_t addr, uint8_t reg, uint16_t* val);
    AaruarduinoError read24(uint8_t addr, uint8_t reg, uint32_t* val);
    AaruarduinoError read32(uint8_t addr, uint8_t reg, uint32_t* val);
    AaruarduinoError readBytes(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len);

    AaruarduinoError write8(uint8_t addr, uint8_t reg, uint8_t val);
    AaruarduinoError write16(uint8_t addr, uint8_t reg, uint16_t val);
    AaruarduinoError writeBytes(uint8_t addr, uint8_t reg, const uint8_t* buf, uint8_t len);

    AaruarduinoError setBit(uint8_t addr, uint8_t reg, uint8_t bit, bool value);
    AaruarduinoError readBit(uint8_t addr, uint8_t reg, uint8_t bit, bool* value);

    bool probeDevice(const struct AaruI2CDeviceDesc* desc);
    AaruarduinoError initDevice(const struct AaruI2CDeviceDesc* desc);
    AaruarduinoError readDevice(const struct AaruI2CDeviceDesc* desc, float* values, int max_vals);

    AaruarduinoError autoDetectDisplay(uint8_t* found_addr, uint8_t* display_type);

    bool isBegun() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _begun;
    AaruarduinoError _err;
    uint32_t _timeout_ms;
    uint32_t _speed_hz;

    AaruarduinoError doRead(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len);
    AaruarduinoError doWrite(uint8_t addr, uint8_t reg, const uint8_t* buf, uint8_t len);
};

#endif // AARUARDUINO_I2C_ENGINE_H
