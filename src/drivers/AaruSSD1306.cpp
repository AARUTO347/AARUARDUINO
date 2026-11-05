// ============================================================
//  AARUARDUINO SSD1306 Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruSSD1306.h"

AaruarduinoError AaruSSD1306::beginI2C(uint8_t addr, uint8_t w, uint8_t h) {
    return beginI2C(nullptr, addr, w, h);
}
AaruarduinoError AaruSSD1306::beginI2C(AaruI2CEngine* i2c, uint8_t addr, uint8_t w, uint8_t h) {
    _addr = addr; _w = w; _h = h; _use_i2c = true; _i2c = i2c;
    _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    size_t bufsize = ((w + 7) / 8) * h;
    _buffer = new uint8_t[bufsize];
    memset(_buffer, 0, bufsize);
#endif
    return _err;
}

AaruarduinoError AaruSSD1306::beginSPI(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t w, uint8_t h) {
    return beginSPI(nullptr, cs, dc, rst, w, h);
}
AaruarduinoError AaruSSD1306::beginSPI(AaruSPIEngine* spi, uint8_t cs, uint8_t dc, uint8_t rst, uint8_t w, uint8_t h) {
    _cs = cs; _dc = dc; _rst = rst; _w = w; _h = h; _use_i2c = false; _spi = spi;
    _configured = true; _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruSSD1306::displayOn() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruSSD1306::displayOff() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruSSD1306::invert(bool inv) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruSSD1306::setContrast(uint8_t val) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruSSD1306::dim(bool dim) { _err = AARUARDUINO_OK; return _err; }

void AaruSSD1306::clear() {}
void AaruSSD1306::display() {}
void AaruSSD1306::setPixel(int16_t x, int16_t y, bool on) {}
bool AaruSSD1306::getPixel(int16_t x, int16_t y) { return false; }

void AaruSSD1306::setCursor(int16_t x, int16_t y) { _cursor_x = x; _cursor_y = y; }
void AaruSSD1306::setTextSize(uint8_t s) { _text_size = s; }
void AaruSSD1306::setTextColor(bool fg, bool bg) { _text_color_fg = fg; _text_color_bg = bg; }
void AaruSSD1306::setTextWrap(bool w) { _text_wrap = w; }

size_t AaruSSD1306::write(uint8_t c) { return 1; }
size_t AaruSSD1306::print(const char* str) { return strlen(str); }
size_t AaruSSD1306::println(const char* str) { return strlen(str) + 1; }
size_t AaruSSD1306::print(int n, int base) { return 0; }
size_t AaruSSD1306::print(float f, int digits) { return 0; }

void AaruSSD1306::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool color) {}
void AaruSSD1306::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, bool color) {}
void AaruSSD1306::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, bool color) {}
void AaruSSD1306::drawCircle(int16_t x0, int16_t y0, int16_t r, bool color) {}
void AaruSSD1306::fillCircle(int16_t x0, int16_t y0, int16_t r, bool color) {}
void AaruSSD1306::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, bool color) {}

int16_t AaruSSD1306::width() const { return _w; }
int16_t AaruSSD1306::height() const { return _h; }
bool AaruSSD1306::isConfigured() const { return _configured; }
AaruarduinoError AaruSSD1306::lastError() const { return _err; }
const char* AaruSSD1306::errorString() const { return aaruarduinoErrorString(_err); }

uint8_t* AaruSSD1306::getBuffer() { return _buffer; }
size_t AaruSSD1306::bufferSize() const { return ((_w + 7) / 8) * _h; }

AaruarduinoError AaruSSD1306::init() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruSSD1306::sendCommand(uint8_t cmd) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruSSD1306::sendData(const uint8_t* data, uint16_t len) {
    _err = AARUARDUINO_OK; return _err;
}
