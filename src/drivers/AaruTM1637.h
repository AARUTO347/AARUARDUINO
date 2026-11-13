// ============================================================
//  AARUARDUINO TM1637 Driver
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_TM1637_H
#define AARUARDUINO_TM1637_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

#define TM1637_CMD_DATA     0x40
#define TM1637_CMD_ADDR     0xC0
#define TM1637_CMD_DISP     0x80

// Number of digits on the module (4 is the most common TM1637 board).
#define TM1637_DIGITS       4

class AaruTM1637 {
public:
    AaruTM1637() = default;

    AaruarduinoError begin(uint8_t clk, uint8_t dio);
    void setBrightness(uint8_t brightness);
    void setColon(bool on);

    AaruarduinoError display(uint8_t pos, uint8_t digit);
    AaruarduinoError displayRaw(uint8_t pos, uint8_t segments);
    AaruarduinoError clear();
    AaruarduinoError printNumber(int num, bool leading_zeros = false);
    AaruarduinoError printFloat(float num, uint8_t decimals);
    AaruarduinoError printTime(uint8_t hour, uint8_t min, bool colon = true);

    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured = false;
    uint8_t _clk = 0, _dio = 0;
    uint8_t _brightness = 0;
    bool _colon = false;
    AaruarduinoError _err = AARUARDUINO_OK;
    uint8_t _buffer[TM1637_DIGITS] = {};

    void start();
    void stop();
    bool writeByte(uint8_t b);
    void sendCommand(uint8_t cmd);
    static uint8_t encodeDigit(uint8_t d);
};

#endif // AARUARDUINO_TM1637_H
