// ============================================================
//  AARUARDUINO IR Remote Driver (NEC, Sony, RC5, RC6)
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_IR_REMOTE_H
#define AARUARDUINO_IR_REMOTE_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

typedef enum {
    IR_PROTO_UNKNOWN = 0,
    IR_PROTO_NEC     = 1,
    IR_PROTO_SONY    = 2,
    IR_PROTO_RC5     = 3,
    IR_PROTO_RC6     = 4,
} IRProtocol;

struct AaruIRResult {
    IRProtocol protocol;
    uint32_t value;
    uint16_t address;
    uint8_t command;
    bool repeat;
    unsigned long timestamp;
};

class AaruIRRemote {
public:
    AaruIRRemote() = default;

    AaruarduinoError begin(uint8_t recv_pin);
    AaruarduinoError begin(uint8_t recv_pin, IRProtocol proto);

    bool decode(struct AaruIRResult* result);
    void resume();
    bool waitDecode(struct AaruIRResult* result, unsigned long timeout_ms = 5000);

    AaruarduinoError beginSend(uint8_t send_pin);
    AaruarduinoError sendNEC(uint32_t data, uint8_t nbits = 32);
    AaruarduinoError sendSony(uint32_t data, uint8_t nbits = 12);
    AaruarduinoError sendRC5(uint16_t data, uint8_t nbits = 14);
    AaruarduinoError sendRC6(uint32_t data, uint8_t nbits = 20);
    AaruarduinoError sendRaw(const uint16_t* buf, uint16_t len, uint16_t freq_khz = 38);

    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured;
    uint8_t _recv_pin;
    uint8_t _send_pin;
    int _proto;
    AaruarduinoError _err;

    bool decodeNEC(struct AaruIRResult* r);
    bool decodeSony(struct AaruIRResult* r);
    bool decodeRC5(struct AaruIRResult* r);
    bool decodeRC6(struct AaruIRResult* r);
    void sendMark(uint16_t us);
    void sendSpace(uint16_t us);
    void sendPulse(uint16_t mark, uint16_t space);
};

#endif // AARUARDUINO_IR_REMOTE_H
