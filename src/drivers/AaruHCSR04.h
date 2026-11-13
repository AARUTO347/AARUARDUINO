// ============================================================
//  AARUARDUINO HC-SR04 Ultrasonic Driver
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_HCSR04_H
#define AARUARDUINO_HCSR04_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"
#include "utility/AaruarduinoMath.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

class AaruHCSR04 {
public:
    AaruHCSR04() = default;

    AaruarduinoError begin(uint8_t trig_pin, uint8_t echo_pin);
    void setTimeoutUs(unsigned long timeout_us);

    float readCm();
    float readInches();
    bool readCm(float& cm);

    void trigger();
    bool echoReady();
    float readCmAsync();

    uint8_t trigPin() const;
    uint8_t echoPin() const;
    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured = false;
    uint8_t _trig = 0, _echo = 0;
    unsigned long _timeout_us = 0;
    AaruarduinoError _err = AARUARDUINO_OK;
};

#endif // AARUARDUINO_HCSR04_H
