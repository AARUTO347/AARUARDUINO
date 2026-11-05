// ============================================================
//  AARUARDUINO Digital Engine
//  MADE BY AARUTO
// ============================================================
#include "AaruDigitalEngine.h"

AaruarduinoError AaruDigitalEngine::begin(uint8_t pin, AaruDigitalKind kind,
                                           bool active_low, bool pullup) {
    _pin = pin; _kind = kind; _active_low = active_low; _pullup = pullup;
    _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(pin, pullup ? INPUT_PULLUP : INPUT);
#endif
    return _err;
}

AaruarduinoError AaruDigitalEngine::beginDebounced(uint8_t pin, AaruDigitalKind kind,
                                                    bool active_low, bool pullup,
                                                    uint16_t debounce_ms) {
    AaruarduinoError e = begin(pin, kind, active_low, pullup);
    _debounce_ms = debounce_ms;
    return e;
}

bool AaruDigitalEngine::readState() {
#if defined(ARDUINO)
    bool raw = digitalRead(_pin);
#else
    bool raw = false;
#endif
    if (_active_low) raw = !raw;
    updateDebounce();
    return _stable_state;
}

bool AaruDigitalEngine::readRaw() {
#if defined(ARDUINO)
    return digitalRead(_pin);
#else
    return false;
#endif
}

bool AaruDigitalEngine::pressed() { return readState(); }
bool AaruDigitalEngine::rose() { bool r = _rose_pending; _rose_pending = false; return r; }
bool AaruDigitalEngine::fell() { bool f = _fell_pending; _fell_pending = false; return f; }

int AaruDigitalEngine::readButton() {
    bool st = readState();
    if (st && _rose_pending) { _rose_pending = false; return 1; }
    if (!st && _fell_pending) { _fell_pending = false; return -1; }
    if (st) return 2;
    return 0;
}

unsigned long AaruDigitalEngine::pulseCount() { return _pulse_count; }
void AaruDigitalEngine::resetPulseCount() { _pulse_count = 0; }
float AaruDigitalEngine::readFrequencyHz() { return _freq_hz_cached; }
float AaruDigitalEngine::readRPM(float pulses_per_rev) {
    if (pulses_per_rev <= 0) return 0;
    return _freq_hz_cached * 60.0f / pulses_per_rev;
}
float AaruDigitalEngine::readFlowRateLmin(float pulses_per_litre) {
    if (pulses_per_litre <= 0) pulses_per_litre = _flow_ppl;
    if (pulses_per_litre <= 0) return 0;
    return _freq_hz_cached * 60.0f / pulses_per_litre;
}
float AaruDigitalEngine::readFlowRateLmin() { return readFlowRateLmin(_flow_ppl); }

uint32_t AaruDigitalEngine::readIRNEC() { return _ir_value; }
bool AaruDigitalEngine::irDataReady() { return _ir_ready; }

const char* AaruDigitalEngine::kindName() const { return "Digital"; }

void AaruDigitalEngine::updateDebounce() {
#if defined(ARDUINO)
    unsigned long now = millis();
    bool raw = digitalRead(_pin);
#else
    unsigned long now = 0;
    bool raw = false;
#endif
    if (_active_low) raw = !raw;
    if (raw != _stable_state) {
        if ((now - _last_change_ms) >= _debounce_ms) {
            _last_change_ms = now;
            _stable_state = raw;
            if (raw) _rose_pending = true; else _fell_pending = true;
        }
    } else {
        _last_change_ms = now;
    }
}

void AaruDigitalEngine::handlePulseRead() {}
void AaruDigitalEngine::isrPulse() { _pulse_count++; }
void AaruDigitalEngine::isrIREdge() {}
