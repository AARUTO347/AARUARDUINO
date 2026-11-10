// ============================================================
//  AARUARDUINO MAX7219 Driver
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_MAX7219_H
#define AARUARDUINO_MAX7219_H

#include "AARUARDUINOTypes.h"
#include "engines/AaruSPIEngine.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

#define MAX7219_NOOP        0x00
#define MAX7219_DIGIT0      0x01
#define MAX7219_DIGIT1      0x02
#define MAX7219_DIGIT2      0x03
#define MAX7219_DIGIT3      0x04
#define MAX7219_DIGIT4      0x05
#define MAX7219_DIGIT5      0x06
#define MAX7219_DIGIT6      0x07
#define MAX7219_DIGIT7      0x08
#define MAX7219_DECODEMODE  0x09
#define MAX7219_INTENSITY   0x0A
#define MAX7219_SCANLIMIT   0x0B
#define MAX7219_SHUTDOWN    0x0C
#define MAX7219_DISPLAYTEST 0x0F

class AaruMAX7219 {
public:
    AaruMAX7219() = default;

    AaruarduinoError begin(uint8_t cs_pin, uint8_t num_devices = 1);
    AaruarduinoError begin(class AaruSPIEngine* spi, uint8_t cs_pin, uint8_t num_devices = 1);

    AaruarduinoError shutdown(bool off);
    AaruarduinoError setIntensity(uint8_t intensity);
    AaruarduinoError setScanLimit(uint8_t limit);
    AaruarduinoError setDecodeMode(uint8_t mode);

    AaruarduinoError setDigit(uint8_t device, uint8_t digit, uint8_t value, bool dp = false);
    AaruarduinoError setDigits(uint8_t device, const uint8_t* values, uint8_t count, bool dp = false);
    AaruarduinoError printNumber(uint8_t device, long num, bool leading_zeros = false);
    AaruarduinoError printFloat(uint8_t device, float num, uint8_t decimals);
    AaruarduinoError clear(uint8_t device = 255);

    AaruarduinoError setLed(uint8_t device, uint8_t row, uint8_t col, bool on);
    AaruarduinoError setRow(uint8_t device, uint8_t row, uint8_t val);
    AaruarduinoError setColumn(uint8_t device, uint8_t col, uint8_t val);
    AaruarduinoError setMatrix(uint8_t device, const uint8_t* rows);

    uint8_t numDevices() const;
    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured;
    uint8_t _cs;
    uint8_t _num_devices;
    class AaruSPIEngine* _spi;
    bool _own_spi;
    AaruarduinoError _err;

    AaruarduinoError write(uint8_t device, uint8_t reg, uint8_t data);
};

#endif // AARUARDUINO_MAX7219_H
