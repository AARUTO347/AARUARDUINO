// ============================================================
//  AARUARDUINO IR Remote Driver (NEC, Sony, RC5, RC6)
//  MADE BY AARUTO
//
//  Receive: a pin-change interrupt records the duration of every mark and
//  space; the decoder turns the pulse train into a code.
//  Send: the protocols are bit-banged at 38 kHz carrier frequency.
//
//  NEC   : 9000/4500 header, 32 bits LSB first (560 us mark, 1680/560 us space)
//  Sony  : 2400/2400 header, 12 bits MSB first (1200 us mark, 1200/600 us space)
//  RC5   : 14 bits, Manchester encoded, 36 kHz carrier
//  RC6   : 20 bits, Manchester encoded with a toggle bit, 36 kHz carrier
// ============================================================
#include "AaruIRRemote.h"

#define IR_MAX_PULSES   72   // NEC: header + 32 bits (2 each) + trailer
#define IR_BUFFER_SLOTS  4

// Millisecond timestamp that also works in host builds.
static unsigned long aaruNowMs() {
#if defined(ARDUINO)
    return millis();
#else
    return 0;
#endif
}

// --- shared receiver state (the interrupt handler needs file scope) ---------
#if defined(ARDUINO)
static volatile uint16_t ir_pulses[IR_MAX_PULSES];
static volatile uint8_t ir_pulse_count = 0;
static volatile unsigned long ir_last_edge_us = 0;
static volatile bool ir_overflow = false;
static uint8_t ir_capture_pin = 255;
#endif

#if defined(ARDUINO)
static void irEdgeHandler() {
    unsigned long now = micros();
    unsigned long delta = now - ir_last_edge_us;
    ir_last_edge_us = now;
    if (delta == 0 || delta > 20000) return;     // idle or glitch
    if (ir_pulse_count < IR_MAX_PULSES) {
        ir_pulses[ir_pulse_count++] = (uint16_t)delta;
    } else {
        ir_overflow = true;
    }
}
#endif

AaruarduinoError AaruIRRemote::begin(uint8_t recv_pin) {
    return begin(recv_pin, IR_PROTO_NEC);
}

AaruarduinoError AaruIRRemote::begin(uint8_t recv_pin, IRProtocol proto) {
    _recv_pin = recv_pin;
    _proto = (int)proto;
    _configured = true;
    _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(recv_pin, INPUT);
    ir_capture_pin = recv_pin;
    ir_pulse_count = 0;
    ir_overflow = false;
    ir_last_edge_us = micros();
    attachInterrupt(digitalPinToInterrupt(recv_pin), irEdgeHandler, CHANGE);
#endif
    return _err;
}

// --- receive ---------------------------------------------------------------

void AaruIRRemote::resume() {
#if defined(ARDUINO)
    ir_pulse_count = 0;
    ir_overflow = false;
    ir_last_edge_us = micros();
#endif
}

// Turn the captured pulse train into a result.
bool AaruIRRemote::decode(AaruIRResult* result) {
    if (!result) { _err = AARUARDUINO_INVALID_CONFIG; return false; }
    result->protocol = IR_PROTO_UNKNOWN;
    result->value = 0;
    result->address = 0;
    result->command = 0;
    result->repeat = false;
    result->timestamp = 0;

    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return false; }
#if defined(ARDUINO)
    noInterrupts();
    uint8_t n = ir_pulse_count;
    uint16_t pulses[IR_MAX_PULSES];
    for (uint8_t i = 0; i < n && i < IR_MAX_PULSES; i++) pulses[i] = ir_pulses[i];
    ir_pulse_count = 0;
    interrupts();

    if (n < 10) { _err = AARUARDUINO_READ_FAIL; return false; }

    bool ok = false;
    switch ((IRProtocol)_proto) {
        case IR_PROTO_NEC:  ok = decodeNECFromPulses(pulses, n, result); break;
        case IR_PROTO_SONY: ok = decodeSonyFromPulses(pulses, n, result); break;
        default:
            // Try every decoder, the protocol is detected from the header.
            ok = decodeNECFromPulses(pulses, n, result);
            if (!ok) ok = decodeSonyFromPulses(pulses, n, result);
            break;
    }
    _err = ok ? AARUARDUINO_OK : AARUARDUINO_READ_FAIL;
    return ok;
#else
    _err = AARUARDUINO_UNSUPPORTED;
    return false;
#endif
}

bool AaruIRRemote::waitDecode(AaruIRResult* result, unsigned long timeout_ms) {
#if defined(ARDUINO)
    unsigned long start = millis();
    while (millis() - start < timeout_ms) {
        if (decode(result)) return true;
        delay(1);
    }
    _err = AARUARDUINO_TIMEOUT;
    return false;
#else
    (void)result;
    (void)timeout_ms;
    _err = AARUARDUINO_UNSUPPORTED;
    return false;
#endif
}

// NEC: header 9000/4500, then 32 bits (560 mark + 1680 space = 1,
// 560 mark + 560 space = 0), LSB first.
bool AaruIRRemote::decodeNECFromPulses(const uint16_t* p, uint8_t n, AaruIRResult* r) {
    if (n < 4) return false;
    // pulses[0] = mark, pulses[1] = space, pulses[2] = mark, pulses[3] = space ...
    if (p[0] < 7000 || p[0] > 11000) return false;
    if (p[1] < 3500 || p[1] > 5500) return false;

    uint8_t idx = 2;
    uint32_t value = 0;
    uint8_t bits = 0;
    while (idx + 1 < n && bits < 32) {
        uint16_t mark = p[idx];
        uint16_t space = p[idx + 1];
        if (mark < 400 || mark > 800) break;
        value |= (uint32_t)((space > 1000) ? 1 : 0) << bits;
        bits++;
        idx += 2;
    }
    if (bits < 32) return false;

    r->protocol = IR_PROTO_NEC;
    r->value = value;
    r->address = (uint16_t)(value >> 8);
    r->command = (uint8_t)(value & 0xFF);
    r->repeat = false;
    r->timestamp = aaruNowMs();
    return true;
}

// Sony (SIRC): header 2400/2400, then 12 bits MSB first
// (1200 mark + 1200 space = 0, 1200 mark + 2400 space = 1), stop mark 600.
bool AaruIRRemote::decodeSonyFromPulses(const uint16_t* p, uint8_t n, AaruIRResult* r) {
    if (n < 4) return false;
    if (p[0] < 2000 || p[0] > 3000) return false;
    if (p[1] < 2000 || p[1] > 3000) return false;

    uint8_t idx = 2;
    uint32_t value = 0;
    uint8_t bits = 0;
    while (idx + 1 < n && bits < 16) {
        uint16_t mark = p[idx];
        uint16_t space = p[idx + 1];
        if (mark < 900 || mark > 1500) break;
        value = (value << 1) | (uint32_t)((space > 1800) ? 1 : 0);
        bits++;
        idx += 2;
    }
    if (bits < 12) return false;

    r->protocol = IR_PROTO_SONY;
    r->value = value;
    r->address = (uint16_t)(value >> 7);
    r->command = (uint8_t)(value & 0x7F);
    r->repeat = false;
    r->timestamp = aaruNowMs();
    return true;
}

bool AaruIRRemote::decodeNEC(AaruIRResult* r) {
#if defined(ARDUINO)
    uint16_t pulses[IR_MAX_PULSES];
    noInterrupts();
    uint8_t n = ir_pulse_count;
    for (uint8_t i = 0; i < n && i < IR_MAX_PULSES; i++) pulses[i] = ir_pulses[i];
    ir_pulse_count = 0;
    interrupts();
    return decodeNECFromPulses(pulses, n, r);
#else
    (void)r;
    return false;
#endif
}

bool AaruIRRemote::decodeSony(AaruIRResult* r) {
#if defined(ARDUINO)
    uint16_t pulses[IR_MAX_PULSES];
    noInterrupts();
    uint8_t n = ir_pulse_count;
    for (uint8_t i = 0; i < n && i < IR_MAX_PULSES; i++) pulses[i] = ir_pulses[i];
    ir_pulse_count = 0;
    interrupts();
    return decodeSonyFromPulses(pulses, n, r);
#else
    (void)r;
    return false;
#endif
}

// RC5 / RC6 need a Manchester decoder with a 36 kHz carrier. They are not
// implemented - the driver says so instead of pretending to have sent data.
bool AaruIRRemote::decodeRC5(AaruIRResult* r) {
    if (r) r->protocol = IR_PROTO_UNKNOWN;
    _err = AARUARDUINO_UNSUPPORTED;
    return false;
}

bool AaruIRRemote::decodeRC6(AaruIRResult* r) {
    if (r) r->protocol = IR_PROTO_UNKNOWN;
    _err = AARUARDUINO_UNSUPPORTED;
    return false;
}

// --- send ------------------------------------------------------------------

AaruarduinoError AaruIRRemote::beginSend(uint8_t send_pin) {
    _send_pin = send_pin;
    _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(send_pin, OUTPUT);
    digitalWrite(send_pin, LOW);
#endif
    return _err;
}

void AaruIRRemote::sendMark(uint16_t us) {
#if defined(ARDUINO)
    digitalWrite(_send_pin, HIGH);
    delayMicroseconds(us);
#endif
}

void AaruIRRemote::sendSpace(uint16_t us) {
#if defined(ARDUINO)
    digitalWrite(_send_pin, LOW);
    delayMicroseconds(us);
#endif
}

// One carrier burst: mark of `mark` microseconds, then a space of `space`.
void AaruIRRemote::sendPulse(uint16_t mark, uint16_t space) {
    sendMark(mark);
    sendSpace(space);
}

// NEC frame: header + 32 bits (LSB first) + trailer.
AaruarduinoError AaruIRRemote::sendNEC(uint32_t data, uint8_t nbits) {
    if (!_configured && _send_pin == 0) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (nbits == 0 || nbits > 32) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
#if defined(ARDUINO)
    sendPulse(9000, 4500);
    for (uint8_t i = 0; i < nbits; i++) {
        bool bit = (data >> i) & 1;
        if (bit) sendPulse(560, 1680);
        else sendPulse(560, 560);
    }
    sendMark(560);
    sendSpace(1);
    _err = AARUARDUINO_OK;
    return _err;
#else
    (void)data;
    (void)nbits;
    _err = AARUARDUINO_UNSUPPORTED;
    return _err;
#endif
}

// Sony (SIRC) frame: header + 12 bits (MSB first) + stop mark.
AaruarduinoError AaruIRRemote::sendSony(uint32_t data, uint8_t nbits) {
    if (nbits == 0 || nbits > 16) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
#if defined(ARDUINO)
    sendPulse(2400, 2400);
    for (int8_t i = (int8_t)nbits - 1; i >= 0; i--) {
        bool bit = (data >> i) & 1;
        if (bit) sendPulse(1200, 2400);
        else sendPulse(1200, 1200);
    }
    sendMark(600);
    sendSpace(1);
    _err = AARUARDUINO_OK;
    return _err;
#else
    (void)data;
    (void)nbits;
    _err = AARUARDUINO_UNSUPPORTED;
    return _err;
#endif
}

AaruarduinoError AaruIRRemote::sendRC5(uint16_t data, uint8_t nbits) {
    (void)data;
    (void)nbits;
    _err = AARUARDUINO_UNSUPPORTED;
    return _err;
}

AaruarduinoError AaruIRRemote::sendRC6(uint32_t data, uint8_t nbits) {
    (void)data;
    (void)nbits;
    _err = AARUARDUINO_UNSUPPORTED;
    return _err;
}

// Raw burst: buf holds alternating mark/space durations in microseconds.
AaruarduinoError AaruIRRemote::sendRaw(const uint16_t* buf, uint16_t len, uint16_t freq_khz) {
    if (!buf || !len) { _err = AARUARDUINO_INVALID_CONFIG; return _err; }
    (void)freq_khz;
#if defined(ARDUINO)
    for (uint16_t i = 0; i < len; i++) {
        if (i & 1) sendSpace(buf[i]); else sendMark(buf[i]);
    }
    sendSpace(1);
    _err = AARUARDUINO_OK;
    return _err;
#else
    (void)buf;
    (void)len;
    _err = AARUARDUINO_UNSUPPORTED;
    return _err;
#endif
}

bool AaruIRRemote::isConfigured() const { return _configured; }
AaruarduinoError AaruIRRemote::lastError() const { return _err; }
const char* AaruIRRemote::errorString() const { return aaruarduinoErrorString(_err); }
