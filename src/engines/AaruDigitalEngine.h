// ============================================================
//  AARUARDUINO Digital Engine
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_DIGITAL_ENGINE_H
#define AARUARDUINO_DIGITAL_ENGINE_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"
#include "utility/AaruarduinoMath.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

typedef enum {
    AARU_DIG_KIND_NONE           = 0,
    AARU_DIG_KIND_RAW            = 1,
    AARU_DIG_KIND_BUTTON         = 2,
    AARU_DIG_KIND_PIR            = 10,
    AARU_DIG_KIND_RADAR_RCWL     = 11,
    AARU_DIG_KIND_RADAR_LD2410   = 12,
    AARU_DIG_KIND_IR_OBSTACLE    = 20,
    AARU_DIG_KIND_IR_LINE        = 21,
    AARU_DIG_KIND_IR_RECEIVER    = 22,
    AARU_DIG_KIND_FLAME_DIGITAL  = 30,
    AARU_DIG_KIND_SMOKE_DIGITAL  = 31,
    AARU_DIG_KIND_GAS_DIGITAL    = 32,
    AARU_DIG_KIND_TOUCH_TTP223   = 40,
    AARU_DIG_KIND_HALL_DIGITAL   = 50,
    AARU_DIG_KIND_REED_SWITCH    = 51,
    AARU_DIG_KIND_LIMIT_SWITCH   = 52,
    AARU_DIG_KIND_ENDSTOP        = 53,
    AARU_DIG_KIND_VIBRATION_SW420 = 60,
    AARU_DIG_KIND_TILT_SWITCH    = 61,
    AARU_DIG_KIND_SHOCK_SW18010  = 62,
    AARU_DIG_KIND_MERCURY_TILT   = 63,
    AARU_DIG_KIND_PULSE_COUNTER  = 70,
    AARU_DIG_KIND_FLOW_SENSOR    = 71,
    AARU_DIG_KIND_TACHOMETER     = 72,
    AARU_DIG_KIND_KEYPAD_4X4     = 80,
    AARU_DIG_KIND_IR_NEC         = 90,
    AARU_DIG_KIND_SOUND_DETECT   = 100,
    AARU_DIG_KIND_CLAP_SENSOR    = 101,
    AARU_DIG_KIND_LASER_RECEIVE  = 110,
    AARU_DIG_KIND_BEAM_BREAK     = 111,
    AARU_DIG_KIND_DOOR_SENSOR    = 112,
} AaruDigitalKind;

class AaruDigitalEngine {
public:
    AaruDigitalEngine() = default;

    AaruarduinoError begin(uint8_t pin, AaruDigitalKind kind, bool active_low = false, bool pullup = false);
    AaruarduinoError beginDebounced(uint8_t pin, AaruDigitalKind kind,
                                    bool active_low, bool pullup,
                                    uint16_t debounce_ms);

    bool readState();
    bool readRaw();

    bool pressed();
    bool rose();
    bool fell();
    int readButton();

    unsigned long pulseCount();
    void resetPulseCount();
    float readFrequencyHz();
    float readRPM(float pulses_per_rev);
    float readFlowRateLmin(float pulses_per_litre);
    void setFlowPulsesPerLitre(float ppl);
    void setTachPulsesPerRev(float ppr);
    float readFlowRateLmin();
    float readRPM();

    uint32_t readIRNEC();
    bool irDataReady();

    uint8_t getPin() const;
    int getKind() const;
    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;
    const char* kindName() const;
    void setDebounceMs(uint16_t ms);

    void isrPulse();
    void isrIREdge();

private:
    uint8_t _pin;
    int _kind;
    AaruarduinoError _err;
    bool _active_low;
    bool _pullup;
    uint16_t _debounce_ms;
    bool _last_raw;
    bool _stable_state;
    bool _prev_stable;
    bool _rose_pending;
    bool _fell_pending;
    unsigned long _last_change_ms;

    float _flow_ppl;
    float _tach_ppr;

    volatile unsigned long _pulse_count;
    volatile unsigned long _last_pulse_us;
    volatile unsigned long _freq_count_window_start_us;
    volatile unsigned long _freq_count_window;
    float _freq_hz_cached;
    unsigned long _freq_last_calc_us;

    volatile uint16_t _ir_pulses[68];
    volatile int _ir_pulse_idx;
    volatile unsigned long _ir_last_us;
    volatile bool _ir_ready;
    uint32_t _ir_value;

    void updateDebounce();
    void handlePulseRead();
};

#endif // AARUARDUINO_DIGITAL_ENGINE_H
