// ============================================================
//  AARUARDUINO - Missing engine accessors
//  MADE BY AARUTO
//  Implements the small accessor / state methods that are declared in the
//  engine headers. Kept in one place so the engines stay readable.
// ============================================================

#include "AaruAnalogEngine.h"
#include "AaruDigitalEngine.h"
#include "AaruI2CEngine.h"
#include "AaruOneWireEngine.h"
#include "AaruPulseEngine.h"
#include "AaruPWMEngine.h"
#include "AaruSPIEngine.h"

// ------------------------------------------------------------
//  Analog engine
// ------------------------------------------------------------
uint8_t AaruAnalogEngine::getPin() const { return _pin; }
AaruAnalogKind AaruAnalogEngine::getKind() const { return _kind; }
bool AaruAnalogEngine::isConfigured() const { return _kind != AARU_AN_KIND_NONE; }
AaruarduinoError AaruAnalogEngine::lastError() const { return _err; }
const char* AaruAnalogEngine::errorString() const { return aaruarduinoErrorString(_err); }

void AaruAnalogEngine::setParams(float p1, float p2, float p3, float p4) {
    _p1 = p1; _p2 = p2; _p3 = p3; _p4 = p4;
}
void AaruAnalogEngine::setDividerRatio(float ratio) {
    if (ratio > 0.0f) _divider = ratio;
}
void AaruAnalogEngine::setSamples(uint8_t samples) {
    if (samples > 0) _samples = samples;
}
void AaruAnalogEngine::setSettleUs(uint16_t us) { _settle_us = us; }

// The preset table is intentionally empty: every module carries its own
// calibration parameters, so findPreset() reports "no preset" rather than
// returning made-up values.
const struct AaruAnalogPreset* AaruAnalogEngine::findPreset(AaruAnalogKind kind) const {
    (void)kind;
    return nullptr;
}

// ------------------------------------------------------------
//  Digital engine
// ------------------------------------------------------------
void AaruDigitalEngine::setFlowPulsesPerLitre(float ppl) {
    if (ppl > 0.0f) _flow_ppl = ppl;
}
void AaruDigitalEngine::setTachPulsesPerRev(float ppr) {
    if (ppr > 0.0f) _tach_ppr = ppr;
}
uint8_t AaruDigitalEngine::getPin() const { return _pin; }
int AaruDigitalEngine::getKind() const { return _kind; }
bool AaruDigitalEngine::isConfigured() const { return _kind != 0; }
AaruarduinoError AaruDigitalEngine::lastError() const { return _err; }
const char* AaruDigitalEngine::errorString() const { return aaruarduinoErrorString(_err); }
void AaruDigitalEngine::setDebounceMs(uint16_t ms) { _debounce_ms = ms; }

// ------------------------------------------------------------
//  I2C engine
// ------------------------------------------------------------
void AaruI2CEngine::setTimeout(uint32_t timeout_ms) {
    _timeout_ms = timeout_ms;
#if defined(ARDUINO)
    Wire.setTimeout(timeout_ms);
#endif
}
void AaruI2CEngine::setSpeed(uint32_t hz) {
    if (hz == 0) return;
    _speed_hz = hz;
#if defined(ARDUINO)
    Wire.setClock(hz);
#endif
}
bool AaruI2CEngine::isBegun() const { return _begun; }
AaruarduinoError AaruI2CEngine::lastError() const { return _err; }
const char* AaruI2CEngine::errorString() const { return aaruarduinoErrorString(_err); }

// ------------------------------------------------------------
//  OneWire engine
// ------------------------------------------------------------
void AaruOneWireEngine::setTimeoutUs(uint32_t us) { _timeout_us = us; }
bool AaruOneWireEngine::isBegun() const { return _begun; }
uint8_t AaruOneWireEngine::getPin() const { return _pin; }
AaruarduinoError AaruOneWireEngine::lastError() const { return _err; }
const char* AaruOneWireEngine::errorString() const { return aaruarduinoErrorString(_err); }

// ------------------------------------------------------------
//  Pulse engine
// ------------------------------------------------------------
AaruarduinoError AaruPulseEngine::lastError() const { return _err; }
const char* AaruPulseEngine::errorString() const { return aaruarduinoErrorString(_err); }

// ------------------------------------------------------------
//  PWM engine
// ------------------------------------------------------------
bool AaruPWMEngine::isConfigured() const { return _configured; }
uint8_t AaruPWMEngine::getPin() const { return _pin; }
AaruarduinoError AaruPWMEngine::lastError() const { return _err; }
const char* AaruPWMEngine::errorString() const { return aaruarduinoErrorString(_err); }

// ------------------------------------------------------------
//  SPI engine
// ------------------------------------------------------------
void AaruSPIEngine::setDefaultSpeed(uint32_t hz) { _default_speed = hz; }
void AaruSPIEngine::setDefaultMode(uint8_t mode) { _default_mode = mode; }
bool AaruSPIEngine::isBegun() const { return _begun; }
AaruarduinoError AaruSPIEngine::lastError() const { return _err; }
const char* AaruSPIEngine::errorString() const { return aaruarduinoErrorString(_err); }
