// ============================================================
//  AARUARDUINO BME280 / BMP280 Driver
//  MADE BY AARUTO
//
//  Calibration handling and the integer compensation routines follow the
//  Bosch BME280 datasheet reference implementation.
// ============================================================
#include "AaruBME280.h"
#include "utility/AaruarduinoMath.h"

#define BME280_REG_CALIB      0x88
#define BME280_REG_CTRL_HUM   0xF2
#define BME280_REG_CTRL_MEAS  0xF3
#define BME280_REG_CONFIG     0xF4
#define BME280_REG_PRES_MSB   0xF7
#define BME280_REG_TEMP_MSB   0xFA
#define BME280_REG_HUM_MSB    0xFC
#define BME280_REG_CHIP_ID    0xD0

AaruarduinoError AaruBME280::beginI2C(uint8_t addr) {
    return beginI2C(nullptr, addr);
}

AaruarduinoError AaruBME280::beginI2C(AaruI2CEngine* i2c, uint8_t addr) {
    _addr = addr;
    _i2c = i2c;
    _use_i2c = true;
    _configured = true;
    _err = AARUARDUINO_OK;

    if (!_i2c) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }

    // Identify the chip: 0x60 = BME280, 0x58 = BMP280 (no humidity).
    uint8_t id = 0;
    if (readReg(BME280_REG_CHIP_ID, &id) != AARUARDUINO_OK) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }
    _is_bme280 = (id == 0x60);

    if (readCalibration() != AARUARDUINO_OK) return _err;

    // Normal mode, x1 oversampling for temperature, pressure and humidity.
    setOversampling(BME280_OS_1X, BME280_OS_1X, BME280_OS_1X);
    return _err;
}

AaruarduinoError AaruBME280::beginSPI(uint8_t cs_pin) {
    return beginSPI(nullptr, cs_pin);
}

AaruarduinoError AaruBME280::beginSPI(AaruSPIEngine* spi, uint8_t cs_pin) {
    _spi = spi;
    _own_spi = (spi == nullptr);
    _cs = cs_pin;
    _use_i2c = false;
    _configured = true;
    _err = AARUARDUINO_OK;
    if (!_spi) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return _err;
    }
    return _err;
}

AaruarduinoError AaruBME280::setMode(BME280Mode mode) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    uint8_t ctrl = 0;
    if (readReg(BME280_REG_CTRL_MEAS, &ctrl) != AARUARDUINO_OK) return _err;
    ctrl &= 0xE3;               // clear mode bits
    ctrl |= (uint8_t)(mode & 0x03);
    _err = writeReg(BME280_REG_CTRL_MEAS, ctrl);
    return _err;
}

AaruarduinoError AaruBME280::setOversampling(BME280Oversampling temp, BME280Oversampling press, BME280Oversampling hum) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    uint8_t ctrl = 0;
    if (readReg(BME280_REG_CTRL_MEAS, &ctrl) != AARUARDUINO_OK) return _err;
    ctrl &= 0xE3;
    ctrl |= (uint8_t)((temp & 0x07) << 5);
    ctrl |= (uint8_t)((press & 0x07) << 2);
    _err = writeReg(BME280_REG_CTRL_MEAS, ctrl);

    if (_is_bme280) {
        uint8_t ch = 0;
        if (readReg(BME280_REG_CTRL_HUM, &ch) == AARUARDUINO_OK) {
            ch &= 0xE0;
            ch |= (uint8_t)((hum & 0x07) << 5);
            writeReg(BME280_REG_CTRL_HUM, ch);
        }
    }
    return _err;
}

AaruarduinoError AaruBME280::setFilter(BME280Filter filter) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    uint8_t cfg = 0;
    if (readReg(BME280_REG_CONFIG, &cfg) != AARUARDUINO_OK) return _err;
    cfg &= 0xF3;
    cfg |= (uint8_t)((filter & 0x07) << 2);
    _err = writeReg(BME280_REG_CONFIG, cfg);
    return _err;
}

AaruarduinoError AaruBME280::setStandby(uint8_t ms) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    uint8_t standby = 0;
    if (ms <= 1) standby = 0;
    else if (ms <= 10) standby = 1;
    else if (ms <= 25) standby = 2;
    else if (ms <= 50) standby = 3;
    else if (ms <= 100) standby = 4;
    else if (ms <= 250) standby = 5;
    else if (ms <= 500) standby = 6;
    else standby = 7;

    uint8_t cfg = 0;
    if (readReg(BME280_REG_CONFIG, &cfg) != AARUARDUINO_OK) return _err;
    cfg &= 0x8F;
    cfg |= (uint8_t)(standby << 5);
    _err = writeReg(BME280_REG_CONFIG, cfg);
    return _err;
}

AaruarduinoError AaruBME280::writeReg(uint8_t reg, uint8_t val) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    _err = _i2c->write8(_addr, reg, val);
    return _err;
}

AaruarduinoError AaruBME280::readReg(uint8_t reg, uint8_t* val) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    _err = _i2c->read8(_addr, reg, val);
    return _err;
}

AaruarduinoError AaruBME280::readRegs(uint8_t reg, uint8_t* buf, uint8_t len) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    _err = _i2c->readBytes(_addr, reg, buf, len);
    return _err;
}

// --- calibration -----------------------------------------------------------

AaruarduinoError AaruBME280::readCalibration() {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }

    // 0x88 .. 0xA1 : dig_T1..dig_P9 and dig_H1
    uint8_t buf[26];
    if (readRegs(BME280_REG_CALIB, buf, 26) != AARUARDUINO_OK) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }

    _cal.dig_T1 = (uint16_t)((uint16_t)buf[0] | ((uint16_t)buf[1] << 8));
    _cal.dig_T2 = (int16_t)((uint16_t)buf[2] | ((uint16_t)buf[3] << 8));
    _cal.dig_T3 = (int16_t)((uint16_t)buf[4] | ((uint16_t)buf[5] << 8));
    _cal.dig_P1 = (uint16_t)((uint16_t)buf[6] | ((uint16_t)buf[7] << 8));
    _cal.dig_P2 = (int16_t)((uint16_t)buf[8] | ((uint16_t)buf[9] << 8));
    _cal.dig_P3 = (int16_t)((uint16_t)buf[10] | ((uint16_t)buf[11] << 8));
    _cal.dig_P4 = (int16_t)((uint16_t)buf[12] | ((uint16_t)buf[13] << 8));
    _cal.dig_P5 = (int16_t)((uint16_t)buf[14] | ((uint16_t)buf[15] << 8));
    _cal.dig_P6 = (int16_t)((uint16_t)buf[16] | ((uint16_t)buf[17] << 8));
    _cal.dig_P7 = (int16_t)((uint16_t)buf[18] | ((uint16_t)buf[19] << 8));
    _cal.dig_P8 = (int16_t)((uint16_t)buf[20] | ((uint16_t)buf[21] << 8));
    _cal.dig_P9 = (int16_t)((uint16_t)buf[22] | ((uint16_t)buf[23] << 8));

    if (!_is_bme280) {
        // BMP280 has no humidity - nothing more to read.
        _err = AARUARDUINO_OK;
        return _err;
    }

    // 0xA1 : dig_H1, then 0xA2..0xA9 for the rest of the humidity block.
    _cal.dig_H1 = buf[25];

    uint8_t h[8];
    if (readRegs(0xA2, h, 8) != AARUARDUINO_OK) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }
    // h[0]=0xA2 h[1]=0xA3 -> dig_H2 (big endian)
    // h[2]=0xA4             -> dig_H3
    // h[3]=0xA5 h[4]=0xA6    -> dig_H4 (12 bit: h[3]<<4 | h[4]>>4)
    // h[5]=0xA7 h[6]=0xA8    -> dig_H5 (little endian)
    // h[7]=0xA9             -> dig_H6 (signed byte)
    _cal.dig_H2 = (int16_t)(((uint16_t)h[0] << 8) | (uint16_t)h[1]);
    _cal.dig_H3 = h[2];
    int16_t h4 = (int16_t)(((uint16_t)h[3] << 4) | ((uint16_t)h[4] >> 4));
    if (h4 & 0x0800) h4 = (int16_t)(h4 | 0xF000);   // sign extend 12 bit
    _cal.dig_H4 = h4;
    _cal.dig_H5 = (int16_t)(((uint16_t)h[6] << 8) | (uint16_t)h[5]);
    _cal.dig_H6 = (int8_t)h[7];

    _err = AARUARDUINO_OK;
    return _err;
}

// --- raw values ------------------------------------------------------------

AaruarduinoError AaruBME280::readRaw(int32_t* t, int32_t* p, int32_t* h) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }

    uint8_t buf[8];
    if (readRegs(BME280_REG_PRES_MSB, buf, 8) != AARUARDUINO_OK) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }

    // Pressure: 20 bit (0xF7..0xF9)
    uint32_t adc_p = ((uint32_t)buf[0] << 12) | ((uint32_t)buf[1] << 4) | (buf[2] >> 4);
    // Temperature: 20 bit (0xFA..0xFB + 0xFC low nibble)
    uint32_t adc_t = ((uint32_t)buf[3] << 12) | ((uint32_t)buf[4] << 4) | (buf[5] >> 4);
    // Humidity: 16 bit (0xFD..0xFE)
    uint32_t adc_h = ((uint32_t)buf[6] << 8) | (uint32_t)buf[7];

    if (p) *p = (int32_t)adc_p;
    if (t) *t = (int32_t)adc_t;
    if (h) *h = (int32_t)adc_h;
    _err = AARUARDUINO_OK;
    return _err;
}

// --- Bosch compensation ----------------------------------------------------

int32_t AaruBME280::compensateTemp(int32_t adc_T) {
    int32_t var1, var2;
    var1 = ((((adc_T >> 3) - ((int32_t)_cal.dig_T1 << 1))) * ((int32_t)_cal.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)_cal.dig_T1)) * ((adc_T >> 4) - ((int32_t)_cal.dig_T1))) >> 12) *
            ((int32_t)_cal.dig_T3)) >> 14;
    _t_fine = var1 + var2;
    return (_t_fine * 5 + 128) >> 8;      // 0.01 degC
}

uint32_t AaruBME280::compensatePress(int32_t adc_P) {
    int64_t var1, var2;

    var1 = ((int64_t)_t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)_cal.dig_P6;
    var2 = var2 + ((var1 * (int64_t)_cal.dig_P5) << 17);
    var2 = var2 + (((int64_t)_cal.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)_cal.dig_P3) >> 8) + ((var1 * (int64_t)_cal.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)_cal.dig_P1) >> 33;

    if (var1 == 0) {
        _err = AARUARDUINO_ERROR;
        return 0;
    }

    int64_t p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)_cal.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)_cal.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)_cal.dig_P7) << 4);
    return (uint32_t)p;                   // Pa
}

uint32_t AaruBME280::compensateHum(int32_t adc_H) {
    // Bosch reference implementation, split into steps so it stays readable
    // and does not rely on deeply nested expressions.
    int32_t v = _t_fine - 76800;

    int32_t num = ((adc_H << 14) - ((int32_t)_cal.dig_H4 << 20) -
                   ((int32_t)_cal.dig_H5 * v) + 16384) >> 15;

    int32_t a = (v * (int32_t)_cal.dig_H6) >> 10;
    int32_t b = ((v * (int32_t)_cal.dig_H3) >> 11) + 32768;
    int32_t c = (a * b) >> 10;
    int32_t d = c + 2097152;
    int32_t e = (d * (int32_t)_cal.dig_H2 + 8192) >> 14;

    v = num * e;
    v = v - (((((v >> 15) * (v >> 15)) >> 7) * (int32_t)_cal.dig_H1) >> 4);
    v = (v < 0) ? 0 : v;
    v = (v > 419430400) ? 419430400 : v;
    return (uint32_t)(v >> 12);        // %RH * 1024
}

// --- public read -----------------------------------------------------------

bool AaruBME280::read(AaruBME280Data& data) {
    data.temperature = NAN;
    data.humidity = NAN;
    data.pressure = NAN;
    data.altitude = NAN;

    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return false; }

    int32_t adc_t = 0, adc_p = 0, adc_h = 0;
    if (readRaw(&adc_t, &adc_p, &adc_h) != AARUARDUINO_OK) return false;

    int32_t t = compensateTemp(adc_t);
    data.temperature = (float)t / 100.0f;

    uint32_t p = compensatePress(adc_p);
    data.pressure = (float)p / 100.0f;    // hPa

    if (_is_bme280) {
        uint32_t h = compensateHum(adc_h);
        data.humidity = (float)h / 1024.0f;
    }

    _err = AARUARDUINO_OK;
    return true;
}

float AaruBME280::readTemp() {
    AaruBME280Data d;
    if (!read(d)) return NAN;
    return d.temperature;
}

float AaruBME280::readHumidity() {
    AaruBME280Data d;
    if (!read(d)) return NAN;
    if (!_is_bme280) return NAN;
    return d.humidity;
}

float AaruBME280::readPressure() {
    AaruBME280Data d;
    if (!read(d)) return NAN;
    return d.pressure;
}

float AaruBME280::readAltitude(float sea_level_hpa) {
    float pressure = readPressure();
    return aaruarduinoAltitudeFromPressure(pressure, sea_level_hpa);
}

AaruarduinoError AaruBME280::takeForcedMeasurement() {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    uint8_t ctrl = 0;
    if (readReg(BME280_REG_CTRL_MEAS, &ctrl) != AARUARDUINO_OK) return _err;
    ctrl &= 0xE3;
    ctrl |= 0x01;                        // forced mode
    _err = writeReg(BME280_REG_CTRL_MEAS, ctrl);
    return _err;
}

float AaruBME280::seaLevelForAltitude(float altitude_m, float pressure_hpa) {
    if (pressure_hpa <= 0.0f) return 0.0f;
    // Invert the barometric formula: P0 = P / (1 - h/44330)^(1/0.1903)
    float ratio = 1.0f - (altitude_m / 44330.0f);
    if (ratio <= 0.0f) return 0.0f;
    return pressure_hpa / powf(ratio, 1.0f / 0.1903f);
}

bool AaruBME280::isBME280() const { return _is_bme280; }
bool AaruBME280::isConfigured() const { return _configured; }
uint8_t AaruBME280::addr() const { return _addr; }
AaruarduinoError AaruBME280::lastError() const { return _err; }
const char* AaruBME280::errorString() const { return aaruarduinoErrorString(_err); }

