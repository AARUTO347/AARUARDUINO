// ============================================================
//  AARUARDUINO SSD1306 OLED Driver
//  MADE BY AARUTO
//
//  Supports the 128x64 and 128x32 SSD1306 modules over I2C and SPI.
//  The framebuffer lives in RAM and is pushed to the panel page by page.
//  Text uses a built-in 5x7 font (5 columns per glyph, LSB = top row).
// ============================================================
#include "AaruSSD1306.h"
#include <string.h>

#define SSD1306_SETCONTRAST      0x81
#define SSD1306_DISPLAYALLON_RES 0xA4
#define SSD1306_DISPLAYALLON     0xA5
#define SSD1306_NORMALDISPLAY    0xA6
#define SSD1306_INVERTDISPLAY    0xA7
#define SSD1306_DISPLAYOFF       0xAE
#define SSD1306_DISPLAYON        0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS       0xDA
#define SSD1306_SETVCOMDETECT    0xDB
#define SSD1306_SETDISPLAYCLOCK  0xD5
#define SSD1306_SETPRECHARGEPER  0xD9
#define SSD1306_SETMULTIPLEX     0xA8
#define SSD1306_SETLOWCOLUMN     0x00
#define SSD1306_SETHIGHCOLUMN    0x10
#define SSD1306_SETSTARTLINE     0x40
#define SSD1306_MEMORYMODE       0x20
#define SSD1306_COLUMNADDR       0x21
#define SSD1306_PAGEADDR         0x22
#define SSD1306_COMSCANINC       0xC0
#define SSD1306_COMSCANDEC       0xC8
#define SSD1306_SEGREMAP         0xA0
#define SSD1306_CHARGEPUMP       0x8D
#define SSD1306_EXTERNALVCC      0x1

// 5x7 font, ASCII 0x20..0x7E. Each glyph is 5 columns, bit0 = top pixel.
// Stored in PROGMEM on AVR so it does not consume the 2 kB of RAM.
#if defined(ARDUINO) && defined(__AVR__)
#include <avr/pgmspace.h>
#define AARU_FONT_PROGMEM PROGMEM
#define AARU_READ_FONT(addr) pgm_read_byte(addr)
#else
#define AARU_FONT_PROGMEM
#define AARU_READ_FONT(addr) (*(addr))
#endif

static const uint8_t kFont5x7[][5] AARU_FONT_PROGMEM = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x5F, 0x00, 0x00},   // space !
    {0x00, 0x07, 0x00, 0x07, 0x00}, {0x14, 0x7F, 0x14, 0x7F, 0x14},   // " #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, {0x23, 0x13, 0x08, 0x64, 0x62},   // $ %
    {0x36, 0x49, 0x55, 0x22, 0x50}, {0x00, 0x05, 0x03, 0x00, 0x00},   // & '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, {0x00, 0x41, 0x22, 0x1C, 0x00},   // ( )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, {0x08, 0x08, 0x3E, 0x08, 0x08},   // * +
    {0x00, 0x50, 0x30, 0x00, 0x00}, {0x08, 0x08, 0x08, 0x08, 0x08},   // , -
    {0x00, 0x60, 0x60, 0x00, 0x00}, {0x20, 0x10, 0x08, 0x04, 0x02},   // . /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, {0x00, 0x42, 0x7F, 0x40, 0x00},   // 0 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, {0x21, 0x41, 0x45, 0x4B, 0x31},   // 2 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, {0x27, 0x45, 0x45, 0x45, 0x39},   // 4 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, {0x01, 0x71, 0x09, 0x05, 0x03},   // 6 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, {0x06, 0x49, 0x49, 0x29, 0x1E},   // 8 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, {0x00, 0x56, 0x36, 0x00, 0x00},   // : ;
    {0x00, 0x08, 0x14, 0x22, 0x41}, {0x14, 0x14, 0x14, 0x14, 0x14},   // < =
    {0x41, 0x22, 0x14, 0x08, 0x00}, {0x02, 0x01, 0x51, 0x09, 0x06},   // > ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, {0x7E, 0x11, 0x11, 0x11, 0x7E},   // @ A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, {0x3E, 0x41, 0x41, 0x41, 0x22},   // B C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, {0x7F, 0x49, 0x49, 0x49, 0x41},   // D E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, {0x3E, 0x41, 0x49, 0x49, 0x7A},   // F G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, {0x00, 0x41, 0x7F, 0x41, 0x00},   // H I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, {0x7F, 0x08, 0x14, 0x22, 0x41},   // J K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, {0x7F, 0x02, 0x0C, 0x02, 0x7F},   // L M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, {0x3E, 0x41, 0x41, 0x41, 0x3E},   // N O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, {0x3E, 0x41, 0x51, 0x21, 0x5E},   // P Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, {0x46, 0x49, 0x49, 0x49, 0x31},   // R S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, {0x3F, 0x40, 0x40, 0x40, 0x3F},   // T U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, {0x3F, 0x40, 0x38, 0x40, 0x3F},   // V W
    {0x63, 0x14, 0x08, 0x14, 0x63}, {0x07, 0x08, 0x70, 0x08, 0x07},   // X Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, {0x00, 0x7F, 0x41, 0x41, 0x00},   // Z [
    {0x02, 0x04, 0x08, 0x10, 0x20}, {0x00, 0x41, 0x41, 0x7F, 0x00},   // \ ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, {0x40, 0x40, 0x40, 0x40, 0x40},   // ^ _
    {0x00, 0x01, 0x02, 0x04, 0x00}, {0x20, 0x54, 0x54, 0x54, 0x78},   // ` a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, {0x38, 0x44, 0x44, 0x44, 0x20},   // b c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, {0x38, 0x54, 0x54, 0x54, 0x18},   // d e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, {0x0C, 0x52, 0x52, 0x52, 0x3E},   // f g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, {0x00, 0x44, 0x7D, 0x40, 0x00},   // h i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, {0x7F, 0x10, 0x28, 0x44, 0x00},   // j k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, {0x7C, 0x04, 0x18, 0x04, 0x78},   // l m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, {0x38, 0x44, 0x44, 0x44, 0x38},   // n o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, {0x08, 0x14, 0x14, 0x18, 0x7C},   // p q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, {0x48, 0x54, 0x54, 0x54, 0x20},   // r s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, {0x3C, 0x40, 0x40, 0x20, 0x7C},   // t u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, {0x3C, 0x40, 0x30, 0x40, 0x3C},   // v w
    {0x44, 0x28, 0x10, 0x28, 0x44}, {0x0C, 0x50, 0x50, 0x50, 0x3C},   // x y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, {0x00, 0x08, 0x36, 0x41, 0x00},   // z {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, {0x00, 0x41, 0x36, 0x08, 0x00},   // | }
    {0x08, 0x08, 0x2A, 0x1C, 0x08}    // ~
};

#define FONT_FIRST 0x20
#define FONT_LAST  0x7E
#define FONT_WIDTH 6      // 5 columns + 1 spacing column

// --- lifecycle -------------------------------------------------------------

AaruarduinoError AaruSSD1306::beginI2C(uint8_t addr, uint8_t w, uint8_t h) {
    return beginI2C(nullptr, addr, w, h);
}

AaruarduinoError AaruSSD1306::beginI2C(AaruI2CEngine* i2c, uint8_t addr, uint8_t w, uint8_t h) {
    _addr = addr;
    _w = w;
    _h = h;
    _use_i2c = true;
    _i2c = i2c;
    _configured = true;
    _err = AARUARDUINO_OK;
    _text_size = 1;
    _text_color_fg = true;

    if (!_i2c) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }

    delete[] _buffer;
    size_t bufsize = bufferSize();
    _buffer = new uint8_t[bufsize];
    memset(_buffer, 0, bufsize);

    _err = init();
    return _err;
}

AaruarduinoError AaruSSD1306::beginSPI(uint8_t cs, uint8_t dc, uint8_t rst, uint8_t w, uint8_t h) {
    return beginSPI(nullptr, cs, dc, rst, w, h);
}

AaruarduinoError AaruSSD1306::beginSPI(AaruSPIEngine* spi, uint8_t cs, uint8_t dc, uint8_t rst, uint8_t w, uint8_t h) {
    _cs = cs;
    _dc = dc;
    _rst = rst;
    _w = w;
    _h = h;
    _use_i2c = false;
    _spi = spi;
    _configured = true;
    _err = AARUARDUINO_OK;
    _text_size = 1;
    _text_color_fg = true;

    if (!_spi) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }

    delete[] _buffer;
    size_t bufsize = bufferSize();
    _buffer = new uint8_t[bufsize];
    memset(_buffer, 0, bufsize);

    // Hardware reset if a reset pin was given.
#if defined(ARDUINO)
    if (rst != 255) {
        pinMode(rst, OUTPUT);
        digitalWrite(rst, LOW);
        delay(10);
        digitalWrite(rst, HIGH);
        delay(10);
    }
#endif
    _err = init();
    return _err;
}

// Standard SSD1306 power-up sequence.
AaruarduinoError AaruSSD1306::init() {
#if defined(ARDUINO)
    static const uint8_t init_seq[] = {
        SSD1306_DISPLAYOFF,
        SSD1306_SETDISPLAYCLOCK, 0x80,
        SSD1306_SETMULTIPLEX, 0x3F,
        SSD1306_SETDISPLAYOFFSET, 0x00,
        SSD1306_SETSTARTLINE | 0x00,
        SSD1306_CHARGEPUMP, 0x14,
        SSD1306_MEMORYMODE, 0x00,
        SSD1306_SEGREMAP | 0xF1,
        SSD1306_COMSCANDEC,
        SSD1306_SETCOMPINS, 0x12,
        SSD1306_SETCONTRAST, 0xCF,
        SSD1306_SETPRECHARGEPER, 0xF1,
        SSD1306_SETVCOMDETECT, 0x40,
        SSD1306_DISPLAYALLON_RES,
        SSD1306_NORMALDISPLAY,
        SSD1306_DISPLAYON
    };
    for (uint8_t i = 0; i < sizeof(init_seq); i++) {
        if (sendCommand(init_seq[i]) != AARUARDUINO_OK) return _err;
    }
#else
    // Host build: there is no panel attached, only the framebuffer is set up.
    // sendCommand()/sendData() report AARUARDUINO_UNSUPPORTED there.
#endif
    clear();
    display();
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruSSD1306::displayOn()  { return sendCommand(SSD1306_DISPLAYON); }
AaruarduinoError AaruSSD1306::displayOff() { return sendCommand(SSD1306_DISPLAYOFF); }
AaruarduinoError AaruSSD1306::invert(bool inv) {
    return sendCommand(inv ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}
AaruarduinoError AaruSSD1306::setContrast(uint8_t val) {
    if (sendCommand(SSD1306_SETCONTRAST) != AARUARDUINO_OK) return _err;
    return sendCommand(val);
}
AaruarduinoError AaruSSD1306::dim(bool dim) {
    return setContrast(dim ? 0x10 : 0xCF);
}

// --- panel transfer --------------------------------------------------------

AaruarduinoError AaruSSD1306::sendCommand(uint8_t cmd) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
#if defined(ARDUINO)
    if (_use_i2c) {
        if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
        // Control byte 0x00 = single command byte.
        if (_i2c->writeBytes(_addr, 0x00, &cmd, 1) != AARUARDUINO_OK) {
            _err = AARUARDUINO_WRITE_FAIL;
            return _err;
        }
    } else {
        if (!_spi) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
        digitalWrite(_dc, LOW);        // command
        digitalWrite(_cs, LOW);
        _spi->transfer(cmd);
        digitalWrite(_cs, HIGH);
    }
    _err = AARUARDUINO_OK;
#else
    (void)cmd;
    _err = AARUARDUINO_UNSUPPORTED;
#endif
    return _err;
}

AaruarduinoError AaruSSD1306::sendData(const uint8_t* data, uint16_t len) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (!data || !len) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
#if defined(ARDUINO)
    if (_use_i2c) {
        if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
        // The I2C engine writes a control byte first, so 0x40 starts a data
        // stream exactly as the SSD1306 expects.
        uint16_t sent = 0;
        while (sent < len) {
            uint8_t chunk = (len - sent > 16) ? 16 : (uint8_t)(len - sent);
            if (_i2c->writeBytes(_addr, 0x40, data + sent, chunk) != AARUARDUINO_OK) {
                _err = AARUARDUINO_WRITE_FAIL;
                return _err;
            }
            sent += chunk;
        }
    } else {
        if (!_spi) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
        digitalWrite(_dc, HIGH);       // data
        digitalWrite(_cs, LOW);
        for (uint16_t i = 0; i < len; i++) _spi->transfer(data[i]);
        digitalWrite(_cs, HIGH);
    }
    _err = AARUARDUINO_OK;
#else
    (void)data;
    (void)len;
    _err = AARUARDUINO_UNSUPPORTED;
#endif
    return _err;
}

// Push the framebuffer to the panel, page by page.
void AaruSSD1306::display() {
    if (!_configured || !_buffer) return;
    const uint8_t pages = (uint8_t)(_h / 8);

    for (uint8_t p = 0; p < pages; p++) {
        // Page address + column range.
        sendCommand(SSD1306_PAGEADDR);
        sendCommand((uint8_t)(p & 0x0F));
        sendCommand(SSD1306_COLUMNADDR);
        sendCommand(0x00);
        sendCommand((uint8_t)(_w - 1));

        // The framebuffer is page major (one page = _w bytes), the panel
        // expects the same page in column order, which it already is.
        sendData(_buffer + (uint16_t)p * _w, (uint16_t)_w);
    }
}

// --- framebuffer -----------------------------------------------------------

void AaruSSD1306::clear() {
    if (!_buffer) return;
    memset(_buffer, 0, bufferSize());
}

void AaruSSD1306::setPixel(int16_t x, int16_t y, bool on) {
    if (!_buffer) return;
    if (x < 0 || x >= _w || y < 0 || y >= _h) return;
    uint16_t idx = (uint16_t)((y / 8) * _w + x);
    uint8_t mask = (uint8_t)(1 << (y & 7));
    if (on) _buffer[idx] |= mask;
    else _buffer[idx] &= (uint8_t)~mask;
}

bool AaruSSD1306::getPixel(int16_t x, int16_t y) {
    if (!_buffer) return false;
    if (x < 0 || x >= _w || y < 0 || y >= _h) return false;
    uint16_t idx = (uint16_t)((y / 8) * _w + x);
    return (_buffer[idx] & (1 << (y & 7))) != 0;
}

// --- text ------------------------------------------------------------------

void AaruSSD1306::setCursor(int16_t x, int16_t y) { _cursor_x = x; _cursor_y = y; }
void AaruSSD1306::setTextSize(uint8_t s) { _text_size = s ? s : 1; }
void AaruSSD1306::setTextColor(bool fg, bool bg) { _text_color_fg = fg; _text_color_bg = bg; }
void AaruSSD1306::setTextWrap(bool w) { _text_wrap = w; }

size_t AaruSSD1306::write(uint8_t c) {
    if (!_buffer) return 0;
    if (c == '\n') {
        _cursor_y += (int16_t)(_text_size * 8);
        _cursor_x = 0;
        return 1;
    }
    if (c == '\r') return 1;

    if (c < FONT_FIRST || c > FONT_LAST) c = '?';

    if (_text_wrap && (_cursor_x + FONT_WIDTH * _text_size > _w)) {
        _cursor_x = 0;
        _cursor_y += (int16_t)(_text_size * 8);
    }

    const uint8_t* glyph = reinterpret_cast<const uint8_t*>(kFont5x7) + (size_t)(c - FONT_FIRST) * 5;
    for (uint8_t col = 0; col < 5; col++) {
        uint8_t g = AARU_READ_FONT(glyph + col);
        for (uint8_t row = 0; row < 8; row++) {
            bool on = (g >> row) & 1;
            for (uint8_t sx = 0; sx < _text_size; sx++) {
                for (uint8_t sy = 0; sy < _text_size; sy++) {
                    int16_t px = (int16_t)(_cursor_x + (col * _text_size) + sx);
                    int16_t py = (int16_t)(_cursor_y + (row * _text_size) + sy);
                    if (_text_color_fg) {
                        setPixel(px, py, on);
                    } else if (on) {
                        setPixel(px, py, false);
                    }
                    if (_text_color_bg) setPixel(px, py, true);
                }
            }
        }
    }

    _cursor_x += (int16_t)(FONT_WIDTH * _text_size);
    return 1;
}

size_t AaruSSD1306::print(const char* str) {
    if (!str) return 0;
    size_t n = 0;
    while (*str) n += write((uint8_t)*str++);
    return n;
}

size_t AaruSSD1306::println(const char* str) {
    size_t n = print(str);
    n += write((uint8_t)'\n');
    return n;
}

size_t AaruSSD1306::print(int n, int base) {
    char buf[34];
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
    uint8_t out = 0;
    if (negative) buf[out++] = '-';
    while (len > 0) buf[out++] = tmp[--len];
    buf[out] = '\0';
    return print(buf);
}

size_t AaruSSD1306::print(float f, int digits) {
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

    char buf[40];
    uint8_t out = 0;
    if (negative) buf[out++] = '-';
    // Integer part
    char tmp[24];
    uint8_t len = 0;
    if (int_part == 0) {
        tmp[len++] = '0';
    } else {
        while (int_part > 0 && len < sizeof(tmp) - 1) {
            tmp[len++] = (char)('0' + (int_part % 10));
            int_part /= 10;
        }
    }
    while (len > 0) buf[out++] = tmp[--len];
    if (digits > 0) {
        buf[out++] = '.';
        unsigned long div = 1;
        for (int i = 0; i < digits - 1; i++) div *= 10;
        for (int i = 0; i < digits; i++) {
            buf[out++] = (char)('0' + ((frac / div) % 10));
            div /= 10;
        }
    }
    buf[out] = '\0';
    return print(buf);
}

// --- graphics --------------------------------------------------------------

void AaruSSD1306::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool color) {
    int16_t dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int16_t dy = (y1 > y0) ? (y0 - y1) : (y1 - y0);
    int16_t sx = (x0 < x1) ? 1 : -1;
    int16_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx + dy;

    for (;;) {
        setPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int16_t e2 = (int16_t)(err << 1);
        if (e2 >= dy) { err = (int16_t)(err + dy); x0 = (int16_t)(x0 + sx); }
        if (e2 <= dx) { err = (int16_t)(err + dx); y0 = (int16_t)(y0 + sy); }
    }
}

void AaruSSD1306::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, bool color) {
    if (w <= 0 || h <= 0) return;
    drawLine(x, y, (int16_t)(x + w - 1), y, color);
    drawLine(x, (int16_t)(y + h - 1), (int16_t)(x + w - 1), (int16_t)(y + h - 1), color);
    drawLine(x, y, x, (int16_t)(y + h - 1), color);
    drawLine((int16_t)(x + w - 1), y, (int16_t)(x + w - 1), (int16_t)(y + h - 1), color);
}

void AaruSSD1306::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, bool color) {
    for (int16_t i = 0; i < w; i++) {
        for (int16_t j = 0; j < h; j++) {
            setPixel((int16_t)(x + i), (int16_t)(y + j), color);
        }
    }
}

void AaruSSD1306::drawCircle(int16_t x0, int16_t y0, int16_t r, bool color) {
    if (r <= 0) return;
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    setPixel(x0, (int16_t)(y0 + r), color);
    setPixel(x0, (int16_t)(y0 - r), color);
    setPixel((int16_t)(x0 + r), y0, color);
    setPixel((int16_t)(x0 - r), y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        setPixel((int16_t)(x0 + x), (int16_t)(y0 + y), color);
        setPixel((int16_t)(x0 - x), (int16_t)(y0 + y), color);
        setPixel((int16_t)(x0 + x), (int16_t)(y0 - y), color);
        setPixel((int16_t)(x0 - x), (int16_t)(y0 - y), color);
        setPixel((int16_t)(x0 + y), (int16_t)(y0 + x), color);
        setPixel((int16_t)(x0 - y), (int16_t)(y0 + x), color);
        setPixel((int16_t)(x0 + y), (int16_t)(y0 - x), color);
        setPixel((int16_t)(x0 - y), (int16_t)(y0 - x), color);
    }
}

void AaruSSD1306::fillCircle(int16_t x0, int16_t y0, int16_t r, bool color) {
    if (r <= 0) return;
    for (int16_t i = -r; i <= r; i++) {
        for (int16_t j = -r; j <= r; j++) {
            if ((int32_t)i * i + (int32_t)j * j <= (int32_t)r * r) {
                setPixel((int16_t)(x0 + i), (int16_t)(y0 + j), color);
            }
        }
    }
}

void AaruSSD1306::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, bool color) {
    if (!bitmap || w <= 0 || h <= 0) return;
    // MSB-first, row major.
    for (int16_t j = 0; j < h; j++) {
        for (int16_t i = 0; i < w; i++) {
            uint16_t idx = (uint16_t)(j * ((w + 7) / 8) + (i / 8));
            bool on = (bitmap[idx] >> (7 - (i & 7))) & 1;
            setPixel((int16_t)(x + i), (int16_t)(y + j), color ? on : !on);
        }
    }
}

// --- accessors -------------------------------------------------------------

int16_t AaruSSD1306::width() const { return _w; }
int16_t AaruSSD1306::height() const { return _h; }
bool AaruSSD1306::isConfigured() const { return _configured; }
AaruarduinoError AaruSSD1306::lastError() const { return _err; }
const char* AaruSSD1306::errorString() const { return aaruarduinoErrorString(_err); }

uint8_t* AaruSSD1306::getBuffer() { return _buffer; }
size_t AaruSSD1306::bufferSize() const { return (size_t)(((uint16_t)_w + 7) / 8) * (uint16_t)_h; }
