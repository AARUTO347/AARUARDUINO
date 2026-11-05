// ============================================================
//  AARUARDUINO TM1637 Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruTM1637.h"

AaruarduinoError AaruTM1637::begin(uint8_t clk, uint8_t dio) {
    _clk = clk; _dio = dio; _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(clk, OUTPUT);
    pinMode(dio, OUTPUT);
#endif
    return _err;
}

void AaruTM1637::setBrightness(uint8_t brightness) { _brightness = brightness; }
void AaruTM1637::setColon(bool on) { _colon = on; }

AaruarduinoError AaruTM1637::display(uint8_t pos, uint8_t digit) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruTM1637::displayRaw(uint8_t pos, uint8_t segments) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruTM1637::clear() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruTM1637::printNumber(int num, bool leading_zeros) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruTM1637::printFloat(float num, uint8_t decimals) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruTM1637::printTime(uint8_t hour, uint8_t min, bool colon) {
    _err = AARUARDUINO_OK; return _err;
}

bool AaruTM1637::isConfigured() const { return _configured; }
AaruarduinoError AaruTM1637::lastError() const { return _err; }
const char* AaruTM1637::errorString() const { return aaruarduinoErrorString(_err); }

void AaruTM1637::start() {}
void AaruTM1637::stop() {}
bool AaruTM1637::writeByte(uint8_t b) { return true; }
void AaruTM1637::sendCommand(uint8_t cmd) {}
uint8_t AaruTM1637::encodeDigit(uint8_t d) { return 0; }
