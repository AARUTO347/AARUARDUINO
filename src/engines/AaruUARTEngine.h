// ============================================================
//  AARUARDUINO UART Engine
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_UART_ENGINE_H
#define AARUARDUINO_UART_ENGINE_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"

#if defined(ARDUINO)
#include <Arduino.h>
#if defined(ESP32)
// ESP32 has a real hardware serial that accepts arbitrary pins.
#include <HardwareSerial.h>
#define AARUARDUINO_HAS_HARDWARE_SERIAL 1
#elif defined(__has_include)
#if __has_include(<SoftwareSerial.h>)
#include <SoftwareSerial.h>
#define AARUARDUINO_HAS_SOFTWARE_SERIAL 1
#endif
#else
#include <SoftwareSerial.h>
#define AARUARDUINO_HAS_SOFTWARE_SERIAL 1
#endif
#else
#include <stdint.h>
#endif

struct AaruUARTConfig {
    int rx_pin;
    int tx_pin;
    uint32_t baud;
    uint8_t config;
    uint16_t timeout_ms;
    int buffer_size;
};

class AaruUARTEngine {
public:
    AaruUARTEngine() = default;

    AaruarduinoError beginHardware(int uart_num, uint32_t baud,
                                   int rx = -1, int tx = -1, uint8_t config = 0x06);
    AaruarduinoError beginSoftware(int rx, int tx, uint32_t baud,
                                   bool invert = false, int buffer_size = 64);

    void setTimeout(uint16_t ms);
    void setBufferSize(int sz);

    int available();
    int peek();
    int read();
    size_t readBytes(uint8_t* buf, size_t len);
    size_t readBytesUntil(char terminator, uint8_t* buf, size_t len);
    size_t write(uint8_t b);
    size_t write(const uint8_t* buf, size_t len);
    void flush();

    bool readLine(char* buf, size_t maxlen);
    static bool nmeaValidate(const char* sentence);

    bool isBegun() const;
    bool isHardware() const;
    int getUartNum() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _begun = false;
    bool _hw = false;
    int _uart_num = 0;
    int _rx = 0, _tx = 0;
    uint32_t _baud = 0;
    uint16_t _timeout_ms = 0;
    AaruarduinoError _err = AARUARDUINO_OK;

#if defined(AARUARDUINO_HAS_SOFTWARE_SERIAL)
    class SoftwareSerial* _soft = nullptr;
#elif defined(AARUARDUINO_HAS_HARDWARE_SERIAL)
    class HardwareSerial* _hw_serial = nullptr;
#endif

    class Stream* getStream();
};

#endif // AARUARDUINO_UART_ENGINE_H
