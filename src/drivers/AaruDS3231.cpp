// ============================================================
//  AARUARDUINO DS3231 RTC Driver
//  MADE BY AARUTO
//
//  Register map (Maxim DS3231):
//    0x00-0x06  date and time   0x07-0x0A  alarm 1
//    0x0B-0x0D  alarm 2          0x0E-0x0F  temperature
//    0x10       control         0x11       status
//    0x18-0x1F  SRAM (8 bytes)
// ============================================================
#include "AaruDS3231.h"

#define DS3231_REG_TIME    0x00
#define DS3231_REG_ALARM1  0x07
#define DS3231_REG_ALARM2  0x0B
#define DS3231_REG_TEMP_MSB 0x0E
#define DS3231_REG_TEMP_LSB 0x0F
#define DS3231_REG_CONTROL 0x10
#define DS3231_REG_STATUS  0x11
#define DS3231_REG_SRAM    0x18

// ---- days since 2000-01-01 for a civil date (Howard Hinnant's algorithm) ----
static uint32_t aaruDaysFromCivil(int y, unsigned m, unsigned d) {
    y -= m <= 2;
    const int era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = (unsigned)(y - era * 400);
    const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return (uint32_t)(era * 146097 + (int)doe - 719468);
}

static void aaruCivilFromDays(int32_t z, int* y, unsigned* m, unsigned* d) {
    z += 719468;
    const int era = (z >= 0 ? z : z - 146096) / 146097;
    const unsigned doe = (unsigned)(z - era * 146097);
    const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    const int yy = (int)(yoe + era * 400);
    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const unsigned mp = (5 * doy + 2) / 153;
    const unsigned dd = doy - (153 * mp + 2) / 5 + 1;
    const unsigned mm = mp + (mp < 10 ? 3 : -9);
    *y = yy + (mm <= 2);
    *m = mm;
    *d = dd;
}

AaruarduinoError AaruDS3231::begin(uint8_t addr) {
    return begin(nullptr, addr);
}

AaruarduinoError AaruDS3231::begin(AaruI2CEngine* i2c, uint8_t addr) {
    _addr = addr;
    _i2c = i2c;
    _configured = true;
    _err = AARUARDUINO_OK;
    if (!_i2c) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }
    // Make sure the oscillator is running and the 24 hour mode is active.
    uint8_t ctrl = 0;
    if (readReg(DS3231_REG_CONTROL, &ctrl) != AARUARDUINO_OK) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }
    ctrl |= 0x40;    // disable the 32 kHz output, keep the oscillator alive
    _err = writeReg(DS3231_REG_CONTROL, ctrl);
    return _err;
}

// --- BCD helpers -----------------------------------------------------------

uint8_t AaruDS3231::decToBcd(uint8_t v) {
    return (uint8_t)(((v / 10) << 4) | (v % 10));
}

uint8_t AaruDS3231::bcdToDec(uint8_t v) {
    return (uint8_t)(((v >> 4) * 10) + (v & 0x0F));
}

// --- register access -------------------------------------------------------

AaruarduinoError AaruDS3231::writeReg(uint8_t reg, uint8_t val) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    _err = _i2c->write8(_addr, reg, val);
    return _err;
}

AaruarduinoError AaruDS3231::readReg(uint8_t reg, uint8_t* val) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    _err = _i2c->read8(_addr, reg, val);
    return _err;
}

AaruarduinoError AaruDS3231::readRegs(uint8_t reg, uint8_t* buf, uint8_t len) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    _err = _i2c->readBytes(_addr, reg, buf, len);
    return _err;
}

// --- date and time ---------------------------------------------------------

AaruarduinoError AaruDS3231::setDateTime(const AaruDateTime& dt) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (dt.month < 1 || dt.month > 12 || dt.day < 1 || dt.day > 31 ||
        dt.hour > 23 || dt.minute > 59 || dt.second > 59) {
        _err = AARUARDUINO_INVALID_CONFIG;
        return _err;
    }

    uint8_t buf[7];
    buf[0] = decToBcd(dt.second);
    buf[1] = decToBcd(dt.minute);
    buf[2] = decToBcd(dt.hour);       // bit5 set -> 24 hour mode
    buf[2] |= 0x40;
    buf[3] = (uint8_t)(dt.dow & 0x07);
    buf[4] = decToBcd(dt.day);
    buf[5] = decToBcd(dt.month);
    buf[6] = decToBcd((uint8_t)(dt.year - 2000));

    if (_i2c->writeBytes(_addr, DS3231_REG_TIME, buf, 7) != AARUARDUINO_OK) {
        _err = AARUARDUINO_WRITE_FAIL;
        return _err;
    }
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruDS3231::getDateTime(AaruDateTime& dt) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    uint8_t buf[7];
    if (readRegs(DS3231_REG_TIME, buf, 7) != AARUARDUINO_OK) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }
    dt.second = bcdToDec((uint8_t)(buf[0] & 0x7F));
    dt.minute = bcdToDec((uint8_t)(buf[1] & 0x7F));
    dt.hour = bcdToDec((uint8_t)(buf[2] & 0x3F));
    if (!(buf[2] & 0x40)) {
        // 12 hour mode: bit5 is the PM flag
        bool pm = (buf[2] & 0x20) != 0;
        dt.hour = (uint8_t)(dt.hour % 12);
        if (pm) dt.hour = (uint8_t)(dt.hour + 12);
    }
    dt.dow = (uint8_t)(buf[3] & 0x07);
    dt.day = bcdToDec((uint8_t)(buf[4] & 0x3F));
    dt.month = bcdToDec((uint8_t)(buf[5] & 0x1F));
    dt.year = (uint16_t)(2000 + bcdToDec(buf[6]));

    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruDS3231::setUnixTime(uint32_t epoch) {
    uint32_t days = epoch / 86400UL;
    uint32_t secs = epoch % 86400UL;
    int y;
    unsigned m, d;
    aaruCivilFromDays((int32_t)days, &y, &m, &d);

    AaruDateTime dt;
    dt.year = (uint16_t)y;
    dt.month = (uint8_t)m;
    dt.day = (uint8_t)d;
    dt.hour = (uint8_t)(secs / 3600UL);
    dt.minute = (uint8_t)((secs / 60UL) % 60UL);
    dt.second = (uint8_t)(secs % 60UL);
    dt.dow = (uint8_t)(((days + 4) % 7) + 1);   // 2000-01-01 was a Saturday
    return setDateTime(dt);
}

uint32_t AaruDS3231::getUnixTime() {
    AaruDateTime dt;
    if (getDateTime(dt) != AARUARDUINO_OK) return 0;
    uint32_t days = aaruDaysFromCivil(dt.year, dt.month, dt.day);
    return days * 86400UL + (uint32_t)dt.hour * 3600UL +
           (uint32_t)dt.minute * 60UL + dt.second;
}

// --- temperature -----------------------------------------------------------

float AaruDS3231::readTempC() {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return NAN; }
    uint8_t msb = 0, lsb = 0;
    if (readReg(DS3231_REG_TEMP_MSB, &msb) != AARUARDUINO_OK ||
        readReg(DS3231_REG_TEMP_LSB, &lsb) != AARUARDUINO_OK) {
        _err = AARUARDUINO_READ_FAIL;
        return NAN;
    }
    int16_t raw = (int16_t)(((msb & 0x7F) << 8) | (lsb & 0xFC));
    if (msb & 0x80) raw = (int16_t)-raw;
    _err = AARUARDUINO_OK;
    return (float)raw / 256.0f;
}

// --- alarms ----------------------------------------------------------------

AaruarduinoError AaruDS3231::setAlarm1(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, bool match_dow) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (sec > 59 || min > 59 || hour > 23) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    uint8_t buf[4];
    buf[0] = decToBcd(sec);
    buf[1] = decToBcd(min);
    buf[2] = decToBcd(hour) | 0x40;                 // 24 hour mode
    buf[3] = match_dow ? (uint8_t)(0x80 | (day & 0x07)) : (uint8_t)(day & 0x3F);
    if (_i2c->writeBytes(_addr, DS3231_REG_ALARM1, buf, 4) != AARUARDUINO_OK) {
        _err = AARUARDUINO_WRITE_FAIL;
        return _err;
    }
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruDS3231::setAlarm2(uint8_t min, uint8_t hour, uint8_t day, bool match_dow) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (min > 59 || hour > 23) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    uint8_t buf[3];
    buf[0] = decToBcd(min);
    buf[1] = decToBcd(hour) | 0x40;
    buf[2] = match_dow ? (uint8_t)(0x80 | (day & 0x07)) : (uint8_t)(day & 0x3F);
    if (_i2c->writeBytes(_addr, DS3231_REG_ALARM2, buf, 3) != AARUARDUINO_OK) {
        _err = AARUARDUINO_WRITE_FAIL;
        return _err;
    }
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruDS3231::clearAlarm(uint8_t alarm_num) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    // The alarm registers are cleared by writing zero to them.
    uint8_t buf[4] = {0, 0, 0, 0};
    if (alarm_num == 1) {
        if (_i2c->writeBytes(_addr, DS3231_REG_ALARM1, buf, 4) != AARUARDUINO_OK) {
            _err = AARUARDUINO_WRITE_FAIL;
            return _err;
        }
    } else {
        if (_i2c->writeBytes(_addr, DS3231_REG_ALARM2, buf, 3) != AARUARDUINO_OK) {
            _err = AARUARDUINO_WRITE_FAIL;
            return _err;
        }
    }
    _err = AARUARDUINO_OK;
    return _err;
}

bool AaruDS3231::alarmTriggered(uint8_t alarm_num) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return false; }
    uint8_t status = 0;
    if (readReg(DS3231_REG_STATUS, &status) != AARUARDUINO_OK) return false;
    if (alarm_num == 1) return (status & 0x08) != 0;
    return (status & 0x10) != 0;
}

// --- misc ------------------------------------------------------------------

AaruarduinoError AaruDS3231::setSquareWave(uint8_t freq) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    uint8_t ctrl = 0;
    if (readReg(DS3231_REG_CONTROL, &ctrl) != AARUARDUINO_OK) return _err;
    ctrl &= (uint8_t)~0x1C;                  // clear EOS and SQW enable
    if (freq == 1) ctrl |= 0x08;             // 1 Hz
    else if (freq == 1000) ctrl |= 0x10;     // 1 kHz
    else if (freq == 4096) ctrl |= 0x18;     // 4 kHz
    // any other value leaves the square wave disabled
    _err = writeReg(DS3231_REG_CONTROL, ctrl);
    return _err;
}

AaruarduinoError AaruDS3231::setAgingOffset(int8_t offset) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (offset < -3 || offset > 7) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    // The offset lives in the two's complement of the temperature LSB.
    uint8_t msb = 0, lsb = 0;
    if (readReg(DS3231_REG_TEMP_MSB, &msb) != AARUARDUINO_OK ||
        readReg(DS3231_REG_TEMP_LSB, &lsb) != AARUARDUINO_OK) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }
    uint8_t new_lsb = (uint8_t)((lsb & 0xFC) | ((uint8_t)offset & 0x03));
    _err = writeReg(DS3231_REG_TEMP_LSB, new_lsb);
    return _err;
}

AaruarduinoError AaruDS3231::writeSRAM(uint8_t offset, const uint8_t* data, uint8_t len) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (!data || !len || offset > 7 || (uint8_t)(offset + len) > 8) {
        _err = AARUARDUINO_INVALID_CONFIG;
        return _err;
    }
    if (_i2c->writeBytes(_addr, (uint8_t)(DS3231_REG_SRAM + offset), data, len) != AARUARDUINO_OK) {
        _err = AARUARDUINO_WRITE_FAIL;
        return _err;
    }
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruDS3231::readSRAM(uint8_t offset, uint8_t* data, uint8_t len) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (!data || !len || offset > 7 || (uint8_t)(offset + len) > 8) {
        _err = AARUARDUINO_INVALID_CONFIG;
        return _err;
    }
    if (_i2c->readBytes(_addr, (uint8_t)(DS3231_REG_SRAM + offset), data, len) != AARUARDUINO_OK) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }
    _err = AARUARDUINO_OK;
    return _err;
}

bool AaruDS3231::isConfigured() const { return _configured; }
AaruarduinoError AaruDS3231::lastError() const { return _err; }
const char* AaruDS3231::errorString() const { return aaruarduinoErrorString(_err); }
