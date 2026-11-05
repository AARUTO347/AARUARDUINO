// ============================================================
//  AARUARDUINO HC-SR04 Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruHCSR04.h"

AaruarduinoError AaruHCSR04::begin(uint8_t trig_pin, uint8_t echo_pin) {
    _trig = trig_pin; _echo = echo_pin; _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    digitalWrite(trig_pin, LOW);
#endif
    return _err;
}

void AaruHCSR04::setTimeoutUs(unsigned long timeout_us) { _timeout_us = timeout_us; }

float AaruHCSR04::readCm() {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return NAN; }
#if defined(ARDUINO)
    digitalWrite(_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trig, LOW);
    unsigned long duration = pulseIn(_echo, HIGH, _timeout_us);
    if (duration == 0) { _err = AARUARDUINO_TIMEOUT; return NAN; }
    _err = AARUARDUINO_OK;
    return duration / 58.0f;
#else
    _err = AARUARDUINO_UNSUPPORTED; return NAN;
#endif
}

float AaruHCSR04::readInches() {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return NAN; }
#if defined(ARDUINO)
    digitalWrite(_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trig, LOW);
    unsigned long duration = pulseIn(_echo, HIGH, _timeout_us);
    if (duration == 0) { _err = AARUARDUINO_TIMEOUT; return NAN; }
    _err = AARUARDUINO_OK;
    return duration / 148.0f;
#else
    _err = AARUARDUINO_UNSUPPORTED; return NAN;
#endif
}

bool AaruHCSR04::readCm(float& cm) {
    cm = readCm();
    return !isnan(cm);
}

void AaruHCSR04::trigger() {}
bool AaruHCSR04::echoReady() { return true; }
float AaruHCSR04::readCmAsync() { return NAN; }

uint8_t AaruHCSR04::trigPin() const { return _trig; }
uint8_t AaruHCSR04::echoPin() const { return _echo; }
bool AaruHCSR04::isConfigured() const { return _configured; }
AaruarduinoError AaruHCSR04::lastError() const { return _err; }
const char* AaruHCSR04::errorString() const { return aaruarduinoErrorString(_err); }
