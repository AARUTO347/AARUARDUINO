// ============================================================
//  AARUARDUINO HD44780 LCD Driver (I2C via PCF8574)
//  MADE BY AARUTO
//
//  The PCF8574 backpack maps the expander pins like this (the most common
//  layout, used by the ubiquitous "LCD1602 I2C module"):
//      bit 0 : backlight
//      bit 1 : enable
//      bit 2 : register select (0 = command, 1 = data)
// ============================================================
#include "AaruLCD.h"

#define LCD_EXP_BACKLIGHT 0x01
#define LCD_EXP_ENABLE    0x02
#define LCD_EXP_RS        0x04

// HD44780 commands
#define LCD_CLEAR_DISPLAY      0x01
#define LCD_RETURN_HOME        0x02
#define LCD_ENTRY_MODE_SET     0x04
#define LCD_DISPLAY_CONTROL    0x08
#define LCD_CURSOR_SHIFT       0x10
#define LCD_FUNCTION_SET       0x20
#define LCD_SET_CGRAM_ADDR     0x40
#define LCD_SET_DDRAM_ADDR     0x80

AaruarduinoError AaruLCD::begin(uint8_t addr, uint8_t cols, uint8_t rows) {
    return begin(nullptr, addr, cols, rows);
}

AaruarduinoError AaruLCD::begin(AaruI2CEngine* i2c, uint8_t addr, uint8_t cols, uint8_t rows) {
    _addr = addr;
    _cols = cols ? cols : 16;
    _rows = rows ? rows : 2;
    _i2c = i2c;
    _configured = true;
    _err = AARUARDUINO_OK;

    if (!_i2c) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }

    // Bring the controller into 4 bit mode.
    _backlight_val = LCD_EXP_BACKLIGHT;
    expanderWrite(0);
    write4bits(0x30, false);
    write4bits(0x30, false);
    write4bits(0x30, false);
    write4bits(0x20, false);          // 4 bit mode

    // Function set: 2 lines / 1 line, 5x8 dots.
    uint8_t lines = (_rows > 1) ? 0x08 : 0x00;
    write4bits((uint8_t)(LCD_FUNCTION_SET | lines), false);

    displayOff();
    clear();
    write4bits((uint8_t)(LCD_ENTRY_MODE_SET | 0x02), false);   // increment, no shift
    displayOn();
    backlightOn();
    return _err;
}

// --- expander --------------------------------------------------------------

AaruarduinoError AaruLCD::expanderWrite(uint8_t val) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    _err = _i2c->write8(_addr, val, 0);
    return _err;
}

void AaruLCD::pulseEnable(uint8_t val) {
    expanderWrite((uint8_t)(val | LCD_EXP_ENABLE | _backlight_val));
#if defined(ARDUINO)
    delayMicroseconds(1);
#endif
    expanderWrite((uint8_t)(val & (uint8_t)~LCD_EXP_ENABLE) | _backlight_val);
#if defined(ARDUINO)
    delayMicroseconds(50);
#endif
}

AaruarduinoError AaruLCD::write4bits(uint8_t val, bool rs) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    uint8_t base = rs ? LCD_EXP_RS : 0x00;
    expanderWrite((uint8_t)(base | (val >> 4)));
#if defined(ARDUINO)
    delayMicroseconds(1);
#endif
    pulseEnable((uint8_t)(base | (val >> 4)));
    expanderWrite((uint8_t)(base | (val & 0x0F)));
#if defined(ARDUINO)
    delayMicroseconds(1);
#endif
    pulseEnable((uint8_t)(base | (val & 0x0F)));
    return _err;
}

// Send a byte as two 4 bit nibbles.
AaruarduinoError AaruLCD::send(uint8_t val, bool rs) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    write4bits((uint8_t)(val & 0xF0), rs);
    write4bits((uint8_t)(val << 4), rs);
    return _err;
}

// --- commands --------------------------------------------------------------

AaruarduinoError AaruLCD::clear() {
    _err = send(LCD_CLEAR_DISPLAY, false);
#if defined(ARDUINO)
    delay(2);      // the clear command needs ~1.5 ms
#endif
    return _err;
}

AaruarduinoError AaruLCD::home() {
    _err = send(LCD_RETURN_HOME, false);
#if defined(ARDUINO)
    delay(2);
#endif
    return _err;
}

AaruarduinoError AaruLCD::setCursor(uint8_t col, uint8_t row) {
    if (col >= _cols || row >= _rows) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    uint8_t offset = (row == 0) ? 0x00 : 0x40;
    _err = send((uint8_t)(LCD_SET_DDRAM_ADDR | (offset + col)), false);
    return _err;
}

AaruarduinoError AaruLCD::displayOn()  { return send((uint8_t)(LCD_DISPLAY_CONTROL | 0x04), false); }
AaruarduinoError AaruLCD::displayOff() { return send((uint8_t)(LCD_DISPLAY_CONTROL | 0x00), false); }
AaruarduinoError AaruLCD::cursorOn()   { return send((uint8_t)(LCD_DISPLAY_CONTROL | 0x02), false); }
AaruarduinoError AaruLCD::cursorOff()  { return send((uint8_t)(LCD_DISPLAY_CONTROL | 0x00), false); }
AaruarduinoError AaruLCD::blinkOn()   { return send((uint8_t)(LCD_DISPLAY_CONTROL | 0x01), false); }
AaruarduinoError AaruLCD::blinkOff()  { return send((uint8_t)(LCD_DISPLAY_CONTROL | 0x00), false); }

AaruarduinoError AaruLCD::backlightOn() {
    _backlight_val = LCD_EXP_BACKLIGHT;
    return expanderWrite(_backlight_val);
}

AaruarduinoError AaruLCD::backlightOff() {
    _backlight_val = 0;
    return expanderWrite(0);
}

AaruarduinoError AaruLCD::setBacklight(uint8_t val) {
    _backlight_val = val ? LCD_EXP_BACKLIGHT : 0;
    return expanderWrite(_backlight_val);
}

// --- data ------------------------------------------------------------------

AaruarduinoError AaruLCD::write(uint8_t c) {
    return send(c, true);
}

AaruarduinoError AaruLCD::print(const char* str) {
    if (!str) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    while (*str) {
        _err = write((uint8_t)*str++);
        if (_err != AARUARDUINO_OK) return _err;
    }
    return _err;
}

AaruarduinoError AaruLCD::println(const char* str) {
    _err = print(str);
    if (_err != AARUARDUINO_OK) return _err;
    return write((uint8_t)'\n');
}

AaruarduinoError AaruLCD::print(int n, int base) {
    if (base < 2 || base > 36) base = 10;
    bool negative = (n < 0 && base == 10);
    unsigned long v = negative ? (unsigned long)(-(long)n) : (unsigned long)n;

    char tmp[34];
    uint8_t len = 0;
    if (v == 0) {
        tmp[len++] = '0';
    } else {
        while (v > 0 && len < sizeof(tmp) - 1) {
            uint8_t d = (uint8_t)(v % (unsigned long)base);
            tmp[len++] = (d < 10) ? (char)('0' + d) : (char)('A' + d - 10);
            v /= (unsigned long)base;
        }
    }
    if (negative) {
        _err = write((uint8_t)'-');
        if (_err != AARUARDUINO_OK) return _err;
    }
    while (len > 0) {
        _err = write((uint8_t)tmp[--len]);
        if (_err != AARUARDUINO_OK) return _err;
    }
    return _err;
}

AaruarduinoError AaruLCD::print(float f, int digits) {
    if (digits < 0) digits = 0;
    if (digits > 6) digits = 6;
    bool negative = f < 0.0f;
    if (negative) f = -f;

    float scale = 1.0f;
    for (int i = 0; i < digits; i++) scale *= 10.0f;
    unsigned long scaled = (unsigned long)(f * scale + 0.5f);

    unsigned long divisor = 1;
    for (int i = 0; i < digits; i++) divisor *= 10;
    unsigned long int_part = scaled / divisor;
    unsigned long frac = scaled % divisor;

    if (negative) {
        _err = write((uint8_t)'-');
        if (_err != AARUARDUINO_OK) return _err;
    }

    char buf[24];
    uint8_t len = 0;
    if (int_part == 0) {
        buf[len++] = '0';
    } else {
        while (int_part > 0 && len < sizeof(buf) - 1) {
            buf[len++] = (char)('0' + (int_part % 10));
            int_part /= 10;
        }
    }
    while (len > 0) {
        _err = write((uint8_t)buf[--len]);
        if (_err != AARUARDUINO_OK) return _err;
    }
    if (digits > 0) {
        _err = write((uint8_t)'.');
        if (_err != AARUARDUINO_OK) return _err;
        unsigned long div = 1;
        for (int i = 0; i < digits - 1; i++) div *= 10;
        for (int i = 0; i < digits; i++) {
            _err = write((uint8_t)('0' + ((frac / div) % 10)));
            if (_err != AARUARDUINO_OK) return _err;
            div /= 10;
        }
    }
    return _err;
}

// --- custom characters -----------------------------------------------------

AaruarduinoError AaruLCD::createChar(uint8_t slot, const uint8_t pattern[8]) {
    if (!pattern) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    if (slot > 7) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    _err = send((uint8_t)(LCD_SET_CGRAM_ADDR | (slot << 3)), false);
    if (_err != AARUARDUINO_OK) return _err;
    for (uint8_t i = 0; i < 8; i++) {
        _err = write(pattern[i]);
        if (_err != AARUARDUINO_OK) return _err;
    }
    return _err;
}

AaruarduinoError AaruLCD::writeChar(uint8_t slot) {
    return write((uint8_t)(slot));
}

// --- scrolling -------------------------------------------------------------

AaruarduinoError AaruLCD::scrollLeft()  { return send((uint8_t)(LCD_CURSOR_SHIFT | 0x08), false); }
AaruarduinoError AaruLCD::scrollRight() { return send((uint8_t)(LCD_CURSOR_SHIFT | 0x18), false); }

AaruarduinoError AaruLCD::autoScroll(bool on) {
    return send((uint8_t)(LCD_ENTRY_MODE_SET | (on ? 0x07 : 0x02)), false);
}

AaruarduinoError AaruLCD::leftToRight()  { return send((uint8_t)(LCD_ENTRY_MODE_SET | 0x06), false); }
AaruarduinoError AaruLCD::rightToLeft() { return send((uint8_t)(LCD_ENTRY_MODE_SET | 0x02), false); }

uint8_t AaruLCD::cols() const { return _cols; }
uint8_t AaruLCD::rows() const { return _rows; }
bool AaruLCD::isConfigured() const { return _configured; }
AaruarduinoError AaruLCD::lastError() const { return _err; }
const char* AaruLCD::errorString() const { return aaruarduinoErrorString(_err); }
