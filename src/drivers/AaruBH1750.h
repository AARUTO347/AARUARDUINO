// ============================================================
//  AARUARDUINO BH1750 Light Sensor Driver
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_BH1750_H
#define AARUARDUINO_BH1750_H

#include "AARUARDUINOTypes.h"
#include "engines/AaruI2CEngine.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

#define BH1750_ADDR_LOW   0x23
#define BH1750_ADDR_HIGH  0x5C

typedef enum {
    BH1750_CONT_H_RES   = 0x10,
    BH1750_CONT_H_RES2  = 0x11,
    BH1750_CONT_L_RES   = 0x13,
    BH1750_ONE_H_RES    = 0x20,
    BH1750_ONE_H_RES2   = 0x21,
    BH1750_ONE_L_RES    = 0x23,
} BH1750Mode;

class AaruBH1750 {
public:
    AaruBH1750() = default;

    AaruarduinoError begin(uint8_t addr = 0x23);
    AaruarduinoError begin(class AaruI2CEngine* i2c, uint8_t addr = 0x23);

    bool read(float& lux);
    float readLux();

    AaruarduinoError setMode(BH1750Mode mode);
    AaruarduinoError setMTreg(uint8_t mtreg);

    AaruarduinoError powerOn();
    AaruarduinoError powerOff();
    AaruarduinoError reset();

    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured = false;
    uint8_t _addr = 0;
    class AaruI2CEngine* _i2c = nullptr;
    bool _own_i2c = false;
    int _mode = 0;
    uint8_t _mtreg = 69;
    AaruarduinoError _err = AARUARDUINO_OK;

    AaruarduinoError writeCmd(uint8_t cmd);
};

#endif // AARUARDUINO_BH1750_H
