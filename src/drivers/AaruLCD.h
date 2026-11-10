// ============================================================
//  AARUARDUINO HD44780 LCD Driver (I2C via PCF8574)
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_LCD_H
#define AARUARDUINO_LCD_H

#include "AARUARDUINOTypes.h"
#include "engines/AaruI2CEngine.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

#define LCD_DEFAULT_ADDR 0x27
#define LCD_DEFAULT_ADDR2 0x3F

class AaruLCD {
public:
    AaruLCD() = default;

    AaruarduinoError begin(uint8_t addr = 0x27, uint8_t cols = 16, uint8_t rows = 2);
    AaruarduinoError begin(class AaruI2CEngine* i2c, uint8_t addr = 0x27, uint8_t cols = 16, uint8_t rows = 2);

    AaruarduinoError clear();
    AaruarduinoError home();
    AaruarduinoError setCursor(uint8_t col, uint8_t row);
    AaruarduinoError displayOn();
    AaruarduinoError displayOff();
    AaruarduinoError cursorOn();
    AaruarduinoError cursorOff();
    AaruarduinoError blinkOn();
    AaruarduinoError blinkOff();
    AaruarduinoError backlightOn();
    AaruarduinoError backlightOff();
    AaruarduinoError setBacklight(uint8_t val);

    AaruarduinoError print(const char* str);
    AaruarduinoError println(const char* str);
    AaruarduinoError print(int n, int base = 10);
    AaruarduinoError print(float f, int digits = 2);
    AaruarduinoError write(uint8_t c);

    AaruarduinoError createChar(uint8_t slot, const uint8_t pattern[8]);
    AaruarduinoError writeChar(uint8_t slot);

    AaruarduinoError scrollLeft();
    AaruarduinoError scrollRight();
    AaruarduinoError autoScroll(bool on);
    AaruarduinoError leftToRight();
    AaruarduinoError rightToLeft();

    uint8_t cols() const;
    uint8_t rows() const;
    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured;
    uint8_t _addr;
    uint8_t _cols, _rows;
    uint8_t _backlight_val;
    class AaruI2CEngine* _i2c;
    bool _own_i2c;
    AaruarduinoError _err;

    enum {
        LCD_CLEAR = 0x01,
        LCD_HOME = 0x02,
        LCD_ENTRY_MODE = 0x04,
        LCD_DISPLAY_CTRL = 0x08,
        LCD_CURSOR_SHIFT = 0x10,
        LCD_FUNCTION_SET = 0x20,
        LCD_SET_CGRAM = 0x40,
        LCD_SET_DDRAM = 0x80,
    };

    AaruarduinoError send(uint8_t val, bool rs);
    AaruarduinoError write4bits(uint8_t val, bool rs);
    AaruarduinoError expanderWrite(uint8_t val);
    void pulseEnable(uint8_t val);
};

#endif // AARUARDUINO_LCD_H
