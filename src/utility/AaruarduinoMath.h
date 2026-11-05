// ============================================================
//  AARUARDUINO Pure Math Utilities
//  MADE BY AARUTO
//  Host-testable, no Arduino dependencies.
// ============================================================
#ifndef AARUARDUINO_MATH_H
#define AARUARDUINO_MATH_H

#include <stdint.h>
#include <math.h>

#ifndef NAN
#define NAN (0.0f/0.0f)
#endif

static inline float aaruarduinoDistanceCm(unsigned long echo_us) {
    if (echo_us == 0) return NAN;
    return echo_us / 58.0f;
}

static inline float aaruarduinoDistanceInches(unsigned long echo_us) {
    if (echo_us == 0) return NAN;
    return echo_us / 148.0f;
}

static inline unsigned long aaruarduinoEchoUsFromCm(float cm) {
    if (cm <= 0) return 0;
    return (unsigned long)(cm * 58.0f);
}

static inline int aaruarduinoAdcToMv(int raw, int adc_max, float vref_v, float divider_ratio) {
    if (adc_max <= 0 || vref_v <= 0.0f || divider_ratio <= 0.0f) return -1;
    if (raw < 0 || raw > adc_max) return -1;
    return (int)((raw / (float)adc_max) * vref_v * divider_ratio * 1000.0f);
}

static inline float aaruarduinoPercent(int raw, int max_raw) {
    if (max_raw <= 0 || raw < 0 || raw > max_raw) return NAN;
    return (raw / (float)max_raw) * 100.0f;
}

static inline float aaruarduinoLm35C(int mv) { return mv * 0.1f; }
static inline float aaruarduinoTmp36C(int mv) { return (mv - 500.0f) / 10.0f; }

static inline float aaruarduinoNtcBetaC(int raw, int adc_max, float series_r, float beta, float r0, float t0_c) {
    if (raw <= 0 || raw >= adc_max || series_r <= 0 || beta <= 0 || r0 <= 0) return NAN;
    float r_therm = series_r * ((float)adc_max / raw - 1.0f);
    float inv_t = (1.0f / (t0_c + 273.15f)) + (1.0f / beta) * logf(r_therm / r0);
    return (1.0f / inv_t) - 273.15f;
}

static inline float aaruarduinoAcs712A(int mv, float mv_per_amp, float zero_mv) {
    return (mv - zero_mv) / mv_per_amp;
}

static inline float aaruarduinoMqRatio(int raw, int max_raw) {
    if (raw <= 0 || raw > max_raw) return NAN;
    return ((float)max_raw - raw) / raw;
}

static inline float aaruarduinoMqPpm(float ratio, float r0, float a, float b) {
    if (ratio <= 0 || r0 <= 0) return NAN;
    return a * powf(ratio / r0, b);
}

static inline float aaruarduinoAltitudeFromPressure(float pressure_hpa, float sea_level_hpa) {
    if (pressure_hpa <= 0 || sea_level_hpa <= 0) return NAN;
    return 44330.0f * (1.0f - powf(pressure_hpa / sea_level_hpa, 0.1903f));
}

static inline int aaruarduinoServoUs(int angle, int min_us, int max_us) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    return min_us + ((max_us - min_us) * angle) / 180;
}

static inline unsigned long aaruarduinoStepperStepDelayUs(double rpm, int steps_per_rev) {
    if (rpm <= 0 || steps_per_rev <= 0) return 0;
    return (unsigned long)(60000000.0 / (rpm * steps_per_rev));
}

static inline int aaruarduinoDebounceStep(int stable_state, int raw_state, unsigned long now_ms,
                                          unsigned long* last_change_ms, unsigned long debounce_ms) {
    if (raw_state != stable_state) {
        if ((now_ms - *last_change_ms) >= debounce_ms) {
            *last_change_ms = now_ms;
            return raw_state;
        }
    } else {
        *last_change_ms = now_ms;
    }
    return stable_state;
}

static inline long aaruarduinoMap(long x, long in_min, long in_max, long out_min, long out_max) {
    if (in_max == in_min) return out_min;
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static inline float aaruarduinoMapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    if (in_max == in_min) return out_min;
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static inline int aaruarduinoConstrain(int val, int min_v, int max_v) {
    if (val < min_v) return min_v;
    if (val > max_v) return max_v;
    return val;
}

static inline float aaruarduinoConstrainFloat(float val, float min_v, float max_v) {
    if (val < min_v) return min_v;
    if (val > max_v) return max_v;
    return val;
}

static inline uint8_t aaruarduinoNmeaChecksum(const char* sentence) {
    uint8_t cs = 0;
    if (!sentence) return 0;
    if (*sentence == '$') sentence++;
    while (*sentence && *sentence != '*') {
        cs ^= (uint8_t)*sentence++;
    }
    return cs;
}

static inline const char* aaruarduinoErrorString(AaruarduinoError e) {
    switch (e) {
        case AARUARDUINO_OK:              return "OK";
        case AARUARDUINO_ERROR:           return "Error";
        case AARUARDUINO_INVALID_PIN:     return "Invalid pin";
        case AARUARDUINO_INVALID_CONFIG:  return "Invalid config";
        case AARUARDUINO_TIMEOUT:         return "Timeout";
        case AARUARDUINO_READ_FAIL:       return "Read failed";
        case AARUARDUINO_WRITE_FAIL:      return "Write failed";
        case AARUARDUINO_UNSUPPORTED:     return "Unsupported";
        case AARUARDUINO_MISSING_DEP:     return "Missing dependency";
        case AARUARDUINO_INIT_FAIL:       return "Init failed";
        case AARUARDUINO_NOT_INITIALIZED: return "Not initialized";
        case AARUARDUINO_BUSY:            return "Busy";
        case AARUARDUINO_BUFFER_OVERFLOW: return "Buffer overflow";
        case AARUARDUINO_CRC_ERROR:       return "CRC error";
        case AARUARDUINO_NOT_FOUND:       return "Not found";
        default:                          return "Unknown error";
    }
}

#endif // AARUARDUINO_MATH_H