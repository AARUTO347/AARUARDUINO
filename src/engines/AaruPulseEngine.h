// ============================================================
//  AARUARDUINO Pulse Engine
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_PULSE_ENGINE_H
#define AARUARDUINO_PULSE_ENGINE_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

class AaruPulseEngine {
public:
    AaruPulseEngine() = default;

    unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout_us = 1000000UL);
    unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout_us = 1000000UL);

    AaruarduinoError beginFreqCount(uint8_t pin, unsigned long sample_ms = 1000);
    float readFrequencyHz();
    void endFreqCount();

    AaruarduinoError beginPulseCount(uint8_t pin, bool active_high = true, uint16_t debounce_us = 0);
    unsigned long pulseCount();
    void resetPulseCount();
    float readRateHz();
    float readFlowRateLmin(float pulses_per_litre);

    AaruarduinoError beginEncoder(uint8_t clk, uint8_t dt, int sw_pin = -1);
    long encoderCount();
    void encoderReset();
    int8_t encoderDirection();
    bool encoderButton();

    AaruarduinoError beginPwmIn(uint8_t pin);
    float readPwmDutyCycle();
    float readPwmFrequencyHz();
    void endPwmIn();

    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    AaruarduinoError _err = AARUARDUINO_OK;

    uint8_t _enc_clk = 0, _enc_dt = 0, _enc_sw = 0;
    long _enc_count = 0;
    int8_t _enc_last = 0;
    int8_t _enc_dir = 0;

    uint8_t _pulse_pin = 0;
    volatile unsigned long _pulse_count = {};
    unsigned long _pulse_last_time = 0;
    uint16_t _pulse_debounce_us = 0;

    uint8_t _pwm_pin = 0;
    volatile unsigned long _pwm_high_us = {};
    volatile unsigned long _pwm_period_us = {};
    volatile unsigned long _pwm_last_rise = {};
    volatile bool _pwm_measuring = {};
};

#endif // AARUARDUINO_PULSE_ENGINE_H
