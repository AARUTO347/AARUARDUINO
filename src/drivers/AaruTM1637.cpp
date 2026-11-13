// ============================================================
//  AARUARDUINO TM1637 Driver
//  MADE BY AARUTO
//
//  Two-wire bit-banged TM1637 protocol (START / 8 data bits / ACK / STOP).
//  Segment encoding: bit0 = SEG A ... bit6 = SEG G, bit7 = decimal point.
// ============================================================
#include "AaruTM1637.h"

AaruarduinoError AaruTM1637::begin(uint8_t clk, uint8_t dio) {
    _clk = clk;
    _dio = dio;
    _configured = true;
    _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(clk, OUTPUT);
    pinMode(dio, OUTPUT);
    digitalWrite(clk, HIGH);
    digitalWrite(dio, HIGH);
    // Display off / normal brightness ramp-up.
    sendCommand(TM1637_CMD_DISP | 0x00);
#endif
    return _err;
}

void AaruTM1637::setBrightness(uint8_t brightness) {
    _brightness = (brightness > 15) ? 15 : brightness;
    sendCommand(TM1637_CMD_DISP | 0x08 | _brightness);
}

void AaruTM1637::setColon(bool on) { _colon = on; }

// --- low level protocol ----------------------------------------------------

void AaruTM1637::start() {
#if defined(ARDUINO)
    digitalWrite(_dio, HIGH);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(1);
    digitalWrite(_dio, LOW);      // DIO falls while CLK is high -> START
    delayMicroseconds(1);
#endif
}

void AaruTM1637::stop() {
#if defined(ARDUINO)
    digitalWrite(_clk, HIGH);
    delayMicroseconds(1);
    digitalWrite(_dio, LOW);
    delayMicroseconds(1);
    digitalWrite(_clk, LOW);
    delayMicroseconds(1);
    digitalWrite(_dio, HIGH);     // DIO rises while CLK is high -> STOP
    delayMicroseconds(1);
#endif
}

bool AaruTM1637::writeByte(uint8_t b) {
#if defined(ARDUINO)
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(_clk, LOW);
        if (b & 0x01) digitalWrite(_dio, HIGH); else digitalWrite(_dio, LOW);
        delayMicroseconds(1);
        digitalWrite(_clk, HIGH);
        delayMicroseconds(1);
        b >>= 1;
    }
    // ACK cycle: the device pulls DIO low.
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT);
    delayMicroseconds(1);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(1);
    uint8_t ack = digitalRead(_dio);
    digitalWrite(_clk, LOW);
    pinMode(_dio, OUTPUT);
    digitalWrite(_dio, HIGH);
    return ack == 0;
#else
    (void)b;
    return false;
#endif
}

void AaruTM1637::sendCommand(uint8_t cmd) {
    if (!_configured) return;
    start();
    if (!writeByte(cmd)) {
        _err = AARUARDUINO_WRITE_FAIL;
    }
    stop();
}

// --- segment encoding ------------------------------------------------------

uint8_t AaruTM1637::encodeDigit(uint8_t d) {
    // bit0=A bit1=B bit2=C bit3=D bit4=E bit5=F bit6=G bit7=DP
    static const uint8_t table[10] = {
        0x3F,  // 0
        0x06,  // 1
        0x5B,  // 2
        0x4F,  // 3
        0x66,  // 4
        0x6D,  // 5
        0x7D,  // 6
        0x07,  // 7
        0x7F,  // 8
        0x6F   // 9
    };
    if (d > 9) return 0;
    return table[d];
}

// --- display ---------------------------------------------------------------

AaruarduinoError AaruTM1637::displayRaw(uint8_t pos, uint8_t segments) {
    if (!_configured) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }
    if (pos >= TM1637_DIGITS) {
        _err = AARUARDUINO_INVALID_PIN;
        return _err;
    }
    _buffer[pos] = segments;

    start();
    if (!writeByte(TM1637_CMD_DATA)) {
        stop();
        _err = AARUARDUINO_WRITE_FAIL;
        return _err;
    }
    // The TM1637 shifts every byte one digit to the right, so the rightmost
    // digit has to be sent first.
    for (int8_t i = (int8_t)TM1637_DIGITS - 1; i >= 0; i--) {
        if (!writeByte(_buffer[i])) {
            stop();
            _err = AARUARDUINO_WRITE_FAIL;
            return _err;
        }
    }
    stop();
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruTM1637::display(uint8_t pos, uint8_t digit) {
    uint8_t seg = encodeDigit(digit);
    if (_colon && (pos == 1 || pos == 2)) seg |= 0x80;
    return displayRaw(pos, seg);
}

AaruarduinoError AaruTM1637::clear() {
    for (uint8_t i = 0; i < TM1637_DIGITS; i++) _buffer[i] = 0;
    if (!_configured) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }
    start();
    writeByte(TM1637_CMD_DATA);
    for (uint8_t i = 0; i < TM1637_DIGITS; i++) writeByte(0x00);
    stop();
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruTM1637::printNumber(int num, bool leading_zeros) {
    if (!_configured) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }
    bool negative = num < 0;
    unsigned long v = negative ? (unsigned long)(-(long)num) : (unsigned long)num;

    for (uint8_t i = 0; i < TM1637_DIGITS; i++) {
        uint8_t digit = (uint8_t)(v % 10);
        v /= 10;
        uint8_t seg = encodeDigit(digit);
        if (i == TM1637_DIGITS - 1 && negative) seg = 0x40;   // minus sign
        else if (i == 0 && !leading_zeros && v == 0 && !negative) seg = 0;
        _buffer[i] = seg;
    }

    start();
    if (!writeByte(TM1637_CMD_DATA)) { stop(); _err = AARUARDUINO_WRITE_FAIL; return _err; }
    for (int8_t i = (int8_t)TM1637_DIGITS - 1; i >= 0; i--) {
        if (!writeByte(_buffer[i])) { stop(); _err = AARUARDUINO_WRITE_FAIL; return _err; }
    }
    stop();
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruTM1637::printFloat(float num, uint8_t decimals) {
    if (!_configured) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }
    if (decimals > 3) decimals = 3;

    bool negative = num < 0.0f;
    if (negative) num = -num;

    // Scale to an integer without pulling in printf/sprintf.
    float scale = 1.0f;
    for (uint8_t i = 0; i < decimals; i++) scale *= 10.0f;
    unsigned long scaled = (unsigned long)(num * scale + 0.5f);

    unsigned long divisor = 1;
    for (uint8_t i = 0; i < decimals; i++) divisor *= 10;

    unsigned long int_part = scaled / divisor;
    unsigned long frac_part = scaled % divisor;

    uint8_t used = 0;
    uint8_t tmp[TM1637_DIGITS];
    // Integer part, most significant digit first.
    do {
        tmp[used++] = encodeDigit((uint8_t)(int_part % 10));
        int_part /= 10;
    } while (int_part > 0 && used < TM1637_DIGITS);

    if (decimals > 0) {
        // Fractional digits, least significant first then reversed.
        uint8_t frac[3];
        for (uint8_t i = 0; i < decimals; i++) {
            frac[i] = encodeDigit((uint8_t)(frac_part % 10));
            frac_part /= 10;
        }
        if (used < TM1637_DIGITS) {
            tmp[used++] = frac[decimals - 1] | 0x80;   // decimal point
        }
        for (int8_t i = (int8_t)decimals - 2; i >= 0; i--) {
            if (used >= TM1637_DIGITS) break;
            tmp[used++] = frac[i];
        }
    }

    for (uint8_t i = used; i < TM1637_DIGITS; i++) tmp[i] = 0;

    // Right-align in the buffer.
    for (uint8_t i = 0; i < TM1637_DIGITS; i++) {
        uint8_t idx = (uint8_t)(TM1637_DIGITS - 1 - i);
        _buffer[idx] = tmp[i];
    }
    if (negative && TM1637_DIGITS > used) {
        _buffer[TM1637_DIGITS - 1 - used] = 0x40;
    }

    start();
    if (!writeByte(TM1637_CMD_DATA)) { stop(); _err = AARUARDUINO_WRITE_FAIL; return _err; }
    for (int8_t i = (int8_t)TM1637_DIGITS - 1; i >= 0; i--) {
        if (!writeByte(_buffer[i])) { stop(); _err = AARUARDUINO_WRITE_FAIL; return _err; }
    }
    stop();
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruTM1637::printTime(uint8_t hour, uint8_t min, bool colon) {
    if (!_configured) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }
    if (hour > 23 || min > 59) {
        _err = AARUARDUINO_INVALID_CONFIG;
        return _err;
    }
    _buffer[0] = encodeDigit((uint8_t)(hour / 10));
    _buffer[1] = encodeDigit((uint8_t)(hour % 10));
    _buffer[2] = encodeDigit((uint8_t)(min / 10));
    _buffer[3] = encodeDigit((uint8_t)(min % 10));
    if (colon) _buffer[2] |= 0x80;
    for (uint8_t i = 4; i < TM1637_DIGITS; i++) _buffer[i] = 0;

    start();
    if (!writeByte(TM1637_CMD_DATA)) { stop(); _err = AARUARDUINO_WRITE_FAIL; return _err; }
    for (int8_t i = (int8_t)TM1637_DIGITS - 1; i >= 0; i--) {
        if (!writeByte(_buffer[i])) { stop(); _err = AARUARDUINO_WRITE_FAIL; return _err; }
    }
    stop();
    _err = AARUARDUINO_OK;
    return _err;
}

bool AaruTM1637::isConfigured() const { return _configured; }
AaruarduinoError AaruTM1637::lastError() const { return _err; }
const char* AaruTM1637::errorString() const { return aaruarduinoErrorString(_err); }
