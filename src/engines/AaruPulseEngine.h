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

    unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout_us);
    unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout_us);

    AaruarduinoError beginFreqCount(uint8_t pin, unsigned long sample_ms = 1000);
    float readFrequencyHz();
    void endFreqCount();

    AaruarduinoError beginPulseCount(uint8_t pin, bool active_high, uint16_t debounce_us);
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
    AaruarduinoError _err;

    uint8_t _enc_clk, _enc_dt, _enc_sw;
    long _enc_count;
    int8_t _enc_last;
    int8_t _enc_dir;

    uint8_t _pulse_pin;
    volatile unsigned long _pulse_count;
    unsigned long _pulse_last_time;
    uint16_t _pulse_debounce_us;

    uint8_t _pwm_pin;
    volatile unsigned long _pwm_high_us;
    volatile unsigned long _pwm_period_us;
    volatile unsigned long _pwm_last_rise;
    volatile bool _pwm_measuring;
};

#endif // AARUARDUINO_PULSE_ENGINE_H
