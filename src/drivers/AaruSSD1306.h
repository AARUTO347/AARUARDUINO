// ============================================================
//  AARUARDUINO SSD1306 OLED Driver
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_SSD1306_H
#define AARUARDUINO_SSD1306_H

#include "AARUARDUINOTypes.h"
#include "engines/AaruI2CEngine.h"
#include "engines/AaruSPIEngine.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

#define SSD1306_I2C_ADDR  0x3C
#define SSD1306_I2C_ADDR2 0x3D

#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT 64

class AaruSSD1306 {
public:
    AaruSSD1306() = default;

    AaruarduinoError beginI2C(uint8_t addr = 0x3C, uint8_t w = 128, uint8_t h = 64);
    AaruarduinoError beginI2C(class AaruI2CEngine* i2c, uint8_t addr = 0x3C, uint8_t w = 128, uint8_t h = 64);
    AaruarduinoError beginSPI(uint8_t cs, uint8_t dc, uint8_t rst = 255, uint8_t w = 128, uint8_t h = 64);
    AaruarduinoError beginSPI(class AaruSPIEngine* spi, uint8_t cs, uint8_t dc, uint8_t rst = 255, uint8_t w = 128, uint8_t h = 64);

    AaruarduinoError displayOn();
    AaruarduinoError displayOff();
    AaruarduinoError invert(bool inv);
    AaruarduinoError setContrast(uint8_t val);
    AaruarduinoError dim(bool dim);

    void clear();
    void display();
    void setPixel(int16_t x, int16_t y, bool on);
    bool getPixel(int16_t x, int16_t y);

    void setCursor(int16_t x, int16_t y);
    void setTextSize(uint8_t s);
    void setTextColor(bool fg, bool bg = false);
    void setTextWrap(bool w);
    size_t write(uint8_t c);
    size_t print(const char* str);
    size_t println(const char* str);
    size_t print(int n, int base = 10);
    size_t print(float f, int digits = 2);

    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, bool color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, bool color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, bool color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, bool color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, bool color);

    int16_t width() const;
    int16_t height() const;
    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

    uint8_t* getBuffer();
    size_t bufferSize() const;

private:
    bool _configured = false;
    bool _use_i2c = false;
    uint8_t _addr = 0;
    uint8_t _cs = 0, _dc = 0, _rst = 0;
    int16_t _w = 0, _h = 0;
    class AaruI2CEngine* _i2c = nullptr;
    class AaruSPIEngine* _spi = nullptr;
    bool _own_i2c = false, _own_spi = false;
    uint8_t* _buffer = 0;
    int16_t _cursor_x = 0, _cursor_y = 0;
    uint8_t _text_size = 0;
    bool _text_color_fg = false, _text_color_bg = false;
    bool _text_wrap = false;
    AaruarduinoError _err = AARUARDUINO_OK;

    AaruarduinoError init();
    AaruarduinoError sendCommand(uint8_t cmd);
    AaruarduinoError sendData(const uint8_t* data, uint16_t len);
};

#endif // AARUARDUINO_SSD1306_H
