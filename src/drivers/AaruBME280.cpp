// ============================================================
//  AARUARDUINO BME280 Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruBME280.h"

AaruarduinoError AaruBME280::beginI2C(uint8_t addr) {
    return beginI2C(nullptr, addr);
}
AaruarduinoError AaruBME280::beginI2C(AaruI2CEngine* i2c, uint8_t addr) {
    _addr = addr; _i2c = i2c; _configured = true; _err = AARUARDUINO_OK;
    _is_bme280 = true;
    return _err;
}

AaruarduinoError AaruBME280::setMode(BME280Mode mode) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruBME280::setOversampling(BME280Oversampling temp, BME280Oversampling press, BME280Oversampling hum) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruBME280::setFilter(BME280Filter filter) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruBME280::setStandby(uint8_t ms) {
    _err = AARUARDUINO_OK; return _err;
}

bool AaruBME280::read(AaruBME280Data& data) {
    _err = AARUARDUINO_OK;
    data.temperature = NAN;
    data.humidity = NAN;
    data.pressure = NAN;
    data.altitude = NAN;
    return false;
}

float AaruBME280::readTemp() { return NAN; }
float AaruBME280::readHumidity() { return NAN; }
float AaruBME280::readPressure() { return NAN; }
float AaruBME280::readAltitude(float sea_level_hpa) { return NAN; }

AaruarduinoError AaruBME280::takeForcedMeasurement() {
    _err = AARUARDUINO_OK; return _err;
}
float AaruBME280::seaLevelForAltitude(float altitude_m, float pressure_hpa) { return 0; }

bool AaruBME280::isBME280() const { return _is_bme280; }
bool AaruBME280::isConfigured() const { return _configured; }
uint8_t AaruBME280::addr() const { return _addr; }
AaruarduinoError AaruBME280::lastError() const { return _err; }
const char* AaruBME280::errorString() const { return aaruarduinoErrorString(_err); }

AaruarduinoError AaruBME280::readCalibration() { _err = AARUARDUINO_OK; return _err; }
int32_t AaruBME280::compensateTemp(int32_t adc_T) { return 0; }
uint32_t AaruBME280::compensatePress(int32_t adc_P) { return 0; }
uint32_t AaruBME280::compensateHum(int32_t adc_H) { return 0; }
AaruarduinoError AaruBME280::readRaw(int32_t* t, int32_t* p, int32_t* h) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruBME280::writeReg(uint8_t reg, uint8_t val) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    return _i2c->write8(_addr, reg, val);
}
AaruarduinoError AaruBME280::readReg(uint8_t reg, uint8_t* val) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    return _i2c->read8(_addr, reg, val);
}
AaruarduinoError AaruBME280::readRegs(uint8_t reg, uint8_t* buf, uint8_t len) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    return _i2c->readBytes(_addr, reg, buf, len);
}
