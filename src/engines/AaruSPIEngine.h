// ============================================================
//  AARUARDUINO SPI Engine
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_SPI_ENGINE_H
#define AARUARDUINO_SPI_ENGINE_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"

#if defined(ARDUINO)
#include <Arduino.h>
#include <SPI.h>
#else
#include <stdint.h>
#endif

struct AaruSPIDeviceConfig {
    uint8_t cs_pin;
    uint32_t speed_hz;
    uint8_t mode;
    uint8_t bit_order;
};

class AaruSPIEngine {
public:
    AaruSPIEngine() = default;

    AaruarduinoError begin(int sck, int miso, int mosi);
    void setDefaultSpeed(uint32_t hz);
    void setDefaultMode(uint8_t mode);

    void beginTransaction(const struct AaruSPIDeviceConfig& cfg);
    void endTransaction();

    uint8_t transfer(uint8_t data);
    void transfer(uint8_t* buf, uint16_t len);

    AaruarduinoError writeReg(uint8_t cs, uint8_t reg, uint8_t val, uint32_t speed);
    AaruarduinoError readReg(uint8_t cs, uint8_t reg, uint8_t* val, uint32_t speed);
    AaruarduinoError writeReg16(uint8_t cs, uint8_t reg, uint16_t val, uint32_t speed);
    AaruarduinoError readReg16(uint8_t cs, uint8_t reg, uint16_t* val, uint32_t speed);
    AaruarduinoError readBytes(uint8_t cs, uint8_t reg, uint8_t* buf, uint8_t len, uint32_t speed);
    AaruarduinoError writeBytes(uint8_t cs, const uint8_t* buf, uint8_t len, uint32_t speed);

    AaruarduinoError csPin(uint8_t pin);
    void csSelect(uint8_t pin);
    void csDeselect(uint8_t pin);

    bool isBegun() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _begun;
    AaruarduinoError _err;
    uint32_t _default_speed;
    uint8_t _default_mode;
    int _sck, _miso, _mosi;
};

#endif // AARUARDUINO_SPI_ENGINE_H
