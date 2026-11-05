// ============================================================
//  AARUARDUINO SPI Engine
//  MADE BY AARUTO
// ============================================================
#include "AaruSPIEngine.h"

AaruarduinoError AaruSPIEngine::begin(int sck, int miso, int mosi) {
    _begun = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
#if defined(ARDUINO_ARCH_ESP32)
    SPI.begin(sck, miso, mosi);
#else
    SPI.begin();
#endif
#endif
    return _err;
}

void AaruSPIEngine::beginTransaction(const AaruSPIDeviceConfig& cfg) {
#if defined(ARDUINO)
    SPI.beginTransaction(SPISettings(cfg.speed_hz, (uint8_t)cfg.bit_order, (uint8_t)cfg.mode));
#endif
}
void AaruSPIEngine::endTransaction() {
#if defined(ARDUINO)
    SPI.endTransaction();
#endif
}

uint8_t AaruSPIEngine::transfer(uint8_t data) {
#if defined(ARDUINO)
    return SPI.transfer(data);
#else
    return 0;
#endif
}
void AaruSPIEngine::transfer(uint8_t* buf, uint16_t len) {
#if defined(ARDUINO)
    for (uint16_t i = 0; i < len; i++) buf[i] = SPI.transfer(buf[i]);
#endif
}

AaruarduinoError AaruSPIEngine::writeReg(uint8_t cs, uint8_t reg, uint8_t val, uint32_t speed) {
    uint8_t buf[2] = {reg, val};
    return writeBytes(cs, buf, 2, speed);
}
AaruarduinoError AaruSPIEngine::readReg(uint8_t cs, uint8_t reg, uint8_t* val, uint32_t speed) {
    return readBytes(cs, reg, val, 1, speed);
}
AaruarduinoError AaruSPIEngine::writeReg16(uint8_t cs, uint8_t reg, uint16_t val, uint32_t speed) {
    uint8_t buf[3] = {reg, (uint8_t)(val >> 8), (uint8_t)val};
    return writeBytes(cs, buf, 3, speed);
}
AaruarduinoError AaruSPIEngine::readReg16(uint8_t cs, uint8_t reg, uint16_t* val, uint32_t speed) {
    uint8_t buf[2];
    AaruarduinoError e = readBytes(cs, reg, buf, 2, speed);
    if (e == AARUARDUINO_OK) *val = (buf[0] << 8) | buf[1];
    return e;
}
AaruarduinoError AaruSPIEngine::readBytes(uint8_t cs, uint8_t reg, uint8_t* buf, uint8_t len, uint32_t speed) {
    _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    digitalWrite(cs, LOW);
    SPI.transfer(reg);
    for (uint8_t i = 0; i < len; i++) buf[i] = SPI.transfer(0);
    digitalWrite(cs, HIGH);
#endif
    return _err;
}
AaruarduinoError AaruSPIEngine::writeBytes(uint8_t cs, const uint8_t* buf, uint8_t len, uint32_t speed) {
    _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    digitalWrite(cs, LOW);
    for (uint8_t i = 0; i < len; i++) SPI.transfer(buf[i]);
    digitalWrite(cs, HIGH);
#endif
    return _err;
}

AaruarduinoError AaruSPIEngine::csPin(uint8_t pin) {
    _err = AARUARDUINO_OK;
    return _err;
}
void AaruSPIEngine::csSelect(uint8_t pin) {}
void AaruSPIEngine::csDeselect(uint8_t pin) {}
