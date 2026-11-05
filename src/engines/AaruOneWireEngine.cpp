// ============================================================
//  AARUARDUINO 1-Wire Engine
//  MADE BY AARUTO
// ============================================================
#include "AaruOneWireEngine.h"

AaruarduinoError AaruOneWireEngine::begin(uint8_t pin) {
    _pin = pin; _begun = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(pin, INPUT_PULLUP);
#endif
    return _err;
}

bool AaruOneWireEngine::reset() {
#if defined(ARDUINO)
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delayMicroseconds(480);
    pinMode(_pin, INPUT);
    delayMicroseconds(60);
    uint8_t r = digitalRead(_pin);
    delayMicroseconds(420);
    return r == 0;
#else
    return false;
#endif
}

bool AaruOneWireEngine::resetSearch() { return true; }
AaruarduinoError AaruOneWireEngine::readROM(AaruOneWireROM* rom) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruOneWireEngine::matchROM(const AaruOneWireROM* rom) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruOneWireEngine::skipROM() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruOneWireEngine::searchROM(AaruOneWireROM* rom, AaruOneWireSearchState* state) {
    _err = AARUARDUINO_NOT_FOUND; return _err;
}

uint8_t AaruOneWireEngine::readByte() { return 0; }
void AaruOneWireEngine::writeByte(uint8_t byte) {}
void AaruOneWireEngine::readBytes(uint8_t* buf, uint16_t len) {}
void AaruOneWireEngine::writeBytes(const uint8_t* buf, uint16_t len) {}
uint8_t AaruOneWireEngine::readBit() { return 0; }
void AaruOneWireEngine::writeBit(uint8_t bit) {}

uint8_t AaruOneWireEngine::crc8(const uint8_t* data, uint8_t len) { return 0; }
bool AaruOneWireEngine::checkCRC(const AaruOneWireROM* rom) { return true; }

int AaruOneWireEngine::discover(AaruOneWireROM* roms, int max_devices) { return 0; }

AaruarduinoError AaruOneWireEngine::ds18b20Convert(const AaruOneWireROM* rom) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruOneWireEngine::ds18b20ReadScratchpad(const AaruOneWireROM* rom, uint8_t* scratchpad) {
    _err = AARUARDUINO_OK; return _err;
}
float AaruOneWireEngine::ds18b20ReadTempC(const AaruOneWireROM* rom) { return NAN; }
AaruarduinoError AaruOneWireEngine::ds18b20SetResolution(const AaruOneWireROM* rom, uint8_t resolution) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruOneWireEngine::ds18b20SetAlarms(const AaruOneWireROM* rom, int8_t th, int8_t tl) {
    _err = AARUARDUINO_OK; return _err;
}

void AaruOneWireEngine::write1() {}
void AaruOneWireEngine::write0() {}
uint8_t AaruOneWireEngine::readBitRaw() { return 0; }
void AaruOneWireEngine::delayUs(uint32_t us) {}
