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
#if defined(ESP32) || defined(ESP8266)
#include <HardwareSerial.h>
#else
#include <SoftwareSerial.h>
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
                                   int rx, int tx, uint8_t config);
    AaruarduinoError beginSoftware(int rx, int tx, uint32_t baud,
                                   bool invert, int buffer_size);

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
    bool _begun;
    bool _hw;
    int _uart_num;
    int _rx, _tx;
    uint32_t _baud;
    uint16_t _timeout_ms;
    AaruarduinoError _err;

#if defined(ARDUINO) && !defined(ESP32) && !defined(ESP8266)
    class SoftwareSerial* _soft;
#elif defined(ARDUINO) && (defined(ESP32) || defined(ESP8266))
    class HardwareSerial* _hw_serial;
#endif

    class Stream* getStream();
};

#endif // AARUARDUINO_UART_ENGINE_H
