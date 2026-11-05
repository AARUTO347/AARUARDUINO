// ============================================================
//  AARUARDUINO LCD Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruLCD.h"

AaruarduinoError AaruLCD::begin(uint8_t addr, uint8_t cols, uint8_t rows) {
    return begin(nullptr, addr, cols, rows);
}
AaruarduinoError AaruLCD::begin(AaruI2CEngine* i2c, uint8_t addr, uint8_t cols, uint8_t rows) {
    _addr = addr; _cols = cols; _rows = rows; _i2c = i2c;
    _configured = true; _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruLCD::clear() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::home() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::setCursor(uint8_t col, uint8_t row) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::displayOn() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::displayOff() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::cursorOn() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::cursorOff() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::blinkOn() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::blinkOff() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::backlightOn() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::backlightOff() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::setBacklight(uint8_t val) { _err = AARUARDUINO_OK; return _err; }

AaruarduinoError AaruLCD::print(const char* str) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::println(const char* str) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::print(int n, int base) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::print(float f, int digits) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::write(uint8_t c) { _err = AARUARDUINO_OK; return _err; }

AaruarduinoError AaruLCD::createChar(uint8_t slot, const uint8_t pattern[8]) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruLCD::writeChar(uint8_t slot) { _err = AARUARDUINO_OK; return _err; }

AaruarduinoError AaruLCD::scrollLeft() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::scrollRight() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::autoScroll(bool on) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::leftToRight() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::rightToLeft() { _err = AARUARDUINO_OK; return _err; }

uint8_t AaruLCD::cols() const { return _cols; }
uint8_t AaruLCD::rows() const { return _rows; }
bool AaruLCD::isConfigured() const { return _configured; }
AaruarduinoError AaruLCD::lastError() const { return _err; }
const char* AaruLCD::errorString() const { return aaruarduinoErrorString(_err); }

AaruarduinoError AaruLCD::send(uint8_t val, bool rs) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::write4bits(uint8_t val, bool rs) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruLCD::expanderWrite(uint8_t val) { _err = AARUARDUINO_OK; return _err; }
void AaruLCD::pulseEnable(uint8_t val) {}
