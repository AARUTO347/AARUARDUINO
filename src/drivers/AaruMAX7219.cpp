// ============================================================
//  AARUARDUINO MAX7219 Driver
//  MADE BY AARUTO
//
//  Up to eight MAX7219 modules can be chained. Only the first device in the
//  chain receives the register address, the others get the no-op register so
//  the data cascades through.
// ============================================================
#include "AaruMAX7219.h"

// 10^n without pulling in the double precision math library.
static unsigned long pow10(int n) {
    unsigned long v = 1;
    for (int i = 0; i < n; i++) v *= 10;
    return v;
}

AaruarduinoError AaruMAX7219::begin(uint8_t cs_pin, uint8_t num_devices) {
    return begin(nullptr, cs_pin, num_devices);
}

AaruarduinoError AaruMAX7219::begin(AaruSPIEngine* spi, uint8_t cs_pin, uint8_t num_devices) {
    _cs = cs_pin;
    _num_devices = num_devices ? num_devices : 1;
    _spi = spi;
    _own_spi = (spi == nullptr);
    _configured = true;
    _err = AARUARDUINO_OK;

    if (!_spi) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }

    shutdown(false);          // display on
    setDecodeMode(0);        // no digit decoding, raw segment access
    setScanLimit(7);         // all eight digits
    setIntensity(8);
    clear(255);
    return _err;
}

// Send one register to `device`. The devices behind it receive a no-op so
// the 16 bit frame cascades correctly.
AaruarduinoError AaruMAX7219::write(uint8_t device, uint8_t reg, uint8_t data) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (!_spi) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (device >= _num_devices) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }

    for (uint8_t i = 0; i < _num_devices; i++) {
        uint16_t frame;
        if (i == device) frame = (uint16_t)(((uint16_t)reg << 8) | data);
        else frame = (uint16_t)(((uint16_t)MAX7219_NOOP << 8) | data);
        _spi->transfer((uint8_t)(frame >> 8));
        _spi->transfer((uint8_t)(frame & 0xFF));
    }
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruMAX7219::shutdown(bool off) {
    return write(0, MAX7219_SHUTDOWN, off ? 0x00 : 0x01);
}

AaruarduinoError AaruMAX7219::setIntensity(uint8_t intensity) {
    return write(0, MAX7219_INTENSITY, (uint8_t)(intensity & 0x0F));
}

AaruarduinoError AaruMAX7219::setScanLimit(uint8_t limit) {
    return write(0, MAX7219_SCANLIMIT, (uint8_t)(limit & 0x07));
}

AaruarduinoError AaruMAX7219::setDecodeMode(uint8_t mode) {
    return write(0, MAX7219_DECODEMODE, (uint8_t)(mode & 0x0F));
}

AaruarduinoError AaruMAX7219::setDigit(uint8_t device, uint8_t digit, uint8_t value, bool dp) {
    if (digit > 7) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    return write(device, (uint8_t)(MAX7219_DIGIT0 + digit), (uint8_t)(value | (dp ? 0x80 : 0x00)));
}

AaruarduinoError AaruMAX7219::setDigits(uint8_t device, const uint8_t* values, uint8_t count, bool dp) {
    if (!values) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    if (count > 8) count = 8;
    for (uint8_t i = 0; i < count; i++) {
        AaruarduinoError e = setDigit(device, i, values[i], dp);
        if (e != AARUARDUINO_OK) return e;
    }
    return _err;
}

// Seven segment encoding: bit0=A .. bit6=G, bit7=decimal point.
static uint8_t max7219Segment(uint8_t digit) {
    static const uint8_t table[10] = {
        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
    };
    if (digit > 9) return 0;
    return table[digit];
}

AaruarduinoError AaruMAX7219::printNumber(uint8_t device, long num, bool leading_zeros) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (device >= _num_devices) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }

    bool negative = num < 0;
    unsigned long v = negative ? (unsigned long)(-(long)num) : (unsigned long)num;

    // Digits are written from the least significant one upwards.
    uint8_t seg[8];
    for (uint8_t i = 0; i < 8; i++) {
        seg[i] = max7219Segment((uint8_t)(v % 10));
        v /= 10;
    }
    if (negative) seg[7] = 0x40;    // minus sign

    // Blank the leading digits unless leading zeros were requested.
    if (!leading_zeros) {
        uint8_t first = 0;
        while (first < 7 && seg[first] == 0) { seg[first] = 0; first++; }
        for (uint8_t i = 0; i < first; i++) seg[i] = 0;
    }

    for (uint8_t i = 0; i < 8; i++) {
        AaruarduinoError e = setDigit(device, i, seg[i], false);
        if (e != AARUARDUINO_OK) return e;
    }
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruMAX7219::printFloat(uint8_t device, float num, uint8_t decimals) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (device >= _num_devices) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    if (decimals > 3) decimals = 3;

    bool negative = num < 0.0f;
    if (negative) num = -num;

    float scale = 1.0f;
    for (uint8_t i = 0; i < decimals; i++) scale *= 10.0f;
    unsigned long scaled = (unsigned long)(num * scale + 0.5f);

    unsigned long divisor = 1;
    for (uint8_t i = 0; i < decimals; i++) divisor *= 10;

    unsigned long int_part = scaled / divisor;
    unsigned long frac = scaled % divisor;

    uint8_t seg[8];
    for (uint8_t i = 0; i < 8; i++) seg[i] = 0;

    // Integer part, right aligned in the first four digits.
    uint8_t pos = 0;
    uint8_t tmp[8];
    uint8_t len = 0;
    do {
        tmp[len++] = max7219Segment((uint8_t)(int_part % 10));
        int_part /= 10;
    } while (int_part > 0 && len < 8);
    while (len > 0) {
        seg[pos++] = tmp[--len];
    }
    if (decimals > 0 && pos < 8) {
        seg[pos++] = max7219Segment((uint8_t)(frac % 10)) | 0x80;
        for (int8_t i = (int8_t)decimals - 2; i >= 0 && pos < 8; i--) {
            seg[pos++] = max7219Segment((uint8_t)((frac / (unsigned long)pow10(i)) % 10));
        }
    }
    if (negative && pos < 8) seg[pos++] = 0x40;

    for (uint8_t i = 0; i < 8; i++) {
        AaruarduinoError e = setDigit(device, i, seg[i], false);
        if (e != AARUARDUINO_OK) return e;
    }
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruMAX7219::clear(uint8_t device) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (device == 255) {
        for (uint8_t d = 0; d < _num_devices; d++) {
            for (uint8_t i = 0; i < 8; i++) {
                AaruarduinoError e = setDigit(d, i, 0, false);
                if (e != AARUARDUINO_OK) return e;
            }
        }
        return _err;
    }
    if (device >= _num_devices) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    for (uint8_t i = 0; i < 8; i++) {
        AaruarduinoError e = setDigit(device, i, 0, false);
        if (e != AARUARDUINO_OK) return e;
    }
    return _err;
}

AaruarduinoError AaruMAX7219::setLed(uint8_t device, uint8_t row, uint8_t col, bool on) {
    if (row > 7 || col > 7) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    uint8_t value = (uint8_t)(1 << col);
    return write(device, (uint8_t)(MAX7219_DIGIT0 + row), value);
}

AaruarduinoError AaruMAX7219::setRow(uint8_t device, uint8_t row, uint8_t val) {
    if (row > 7) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    return write(device, (uint8_t)(MAX7219_DIGIT0 + row), val);
}

AaruarduinoError AaruMAX7219::setColumn(uint8_t device, uint8_t col, uint8_t val) {
    if (col > 7) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    // The MAX7219 has no column register, so the columns are combined here.
    uint8_t rows[8];
    for (uint8_t r = 0; r < 8; r++) rows[r] = 0;
    for (uint8_t r = 0; r < 8; r++) {
        if (val & (1 << r)) rows[r] = (uint8_t)(1 << col);
    }
    return setMatrix(device, rows);
}

AaruarduinoError AaruMAX7219::setMatrix(uint8_t device, const uint8_t* rows) {
    if (!rows) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    for (uint8_t r = 0; r < 8; r++) {
        AaruarduinoError e = write(device, (uint8_t)(MAX7219_DIGIT0 + r), rows[r]);
        if (e != AARUARDUINO_OK) return e;
    }
    return _err;
}

uint8_t AaruMAX7219::numDevices() const { return _num_devices; }
bool AaruMAX7219::isConfigured() const { return _configured; }
AaruarduinoError AaruMAX7219::lastError() const { return _err; }
const char* AaruMAX7219::errorString() const { return aaruarduinoErrorString(_err); }
