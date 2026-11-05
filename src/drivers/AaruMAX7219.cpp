// ============================================================
//  AARUARDUINO MAX7219 Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruMAX7219.h"

AaruarduinoError AaruMAX7219::begin(uint8_t cs_pin, uint8_t num_devices) {
    return begin(nullptr, cs_pin, num_devices);
}
AaruarduinoError AaruMAX7219::begin(AaruSPIEngine* spi, uint8_t cs_pin, uint8_t num_devices) {
    _cs = cs_pin; _num_devices = num_devices; _spi = spi;
    _configured = true; _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruMAX7219::shutdown(bool off) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruMAX7219::setIntensity(uint8_t intensity) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruMAX7219::setScanLimit(uint8_t limit) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruMAX7219::setDecodeMode(uint8_t mode) { _err = AARUARDUINO_OK; return _err; }

AaruarduinoError AaruMAX7219::setDigit(uint8_t device, uint8_t digit, uint8_t value, bool dp) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruMAX7219::setDigits(uint8_t device, const uint8_t* values, uint8_t count, bool dp) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruMAX7219::printNumber(uint8_t device, long num, bool leading_zeros) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruMAX7219::printFloat(uint8_t device, float num, uint8_t decimals) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruMAX7219::clear(uint8_t device) { _err = AARUARDUINO_OK; return _err; }

AaruarduinoError AaruMAX7219::setLed(uint8_t device, uint8_t row, uint8_t col, bool on) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruMAX7219::setRow(uint8_t device, uint8_t row, uint8_t val) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruMAX7219::setColumn(uint8_t device, uint8_t col, uint8_t val) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruMAX7219::setMatrix(uint8_t device, const uint8_t* rows) {
    _err = AARUARDUINO_OK; return _err;
}

uint8_t AaruMAX7219::numDevices() const { return _num_devices; }
bool AaruMAX7219::isConfigured() const { return _configured; }
AaruarduinoError AaruMAX7219::lastError() const { return _err; }
const char* AaruMAX7219::errorString() const { return aaruarduinoErrorString(_err); }

AaruarduinoError AaruMAX7219::write(uint8_t device, uint8_t reg, uint8_t data) {
    _err = AARUARDUINO_OK; return _err;
}
