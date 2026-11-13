// ============================================================
//  AARUARDUINO UART Engine
//  MADE BY AARUTO
// ============================================================
#include "AaruUARTEngine.h"
#include <string.h>
#include <stdlib.h>

AaruarduinoError AaruUARTEngine::beginHardware(int uart_num, uint32_t baud,
                                               int rx, int tx, uint8_t config) {
    _begun = true; _hw = true; _uart_num = uart_num; _baud = baud; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    if (rx >= 0 && tx >= 0) {
#if defined(ESP32)
        HardwareSerial* hs = new HardwareSerial(uart_num);
        hs->begin(baud, (uint32_t)config, rx, tx);
        _hw_serial = hs;
#else
        // This core's hardware UART does not take arbitrary pins;
        // use the bundled software serial instead (AVR, ESP8266, SAMD).
#if defined(AARUARDUINO_HAS_SOFTWARE_SERIAL)
        _hw = false;
        _soft = new SoftwareSerial(rx, tx, false);
        _soft->begin(baud);
#else
        Serial.begin(baud);
#endif
#endif
    } else {
        Serial.begin(baud);
    }
#endif
    return _err;
}

AaruarduinoError AaruUARTEngine::beginSoftware(int rx, int tx, uint32_t baud,
                                                bool invert, int buffer_size) {
    _begun = true; _hw = false; _rx = rx; _tx = tx; _baud = baud; _err = AARUARDUINO_OK;
#if defined(AARUARDUINO_HAS_SOFTWARE_SERIAL)
    _soft = new SoftwareSerial(rx, tx, invert);
    _soft->begin(baud);
#elif defined(AARUARDUINO_HAS_HARDWARE_SERIAL)
    // No software serial on this core - fall back to the hardware UART.
    _hw = true;
    _hw_serial = &Serial;
    Serial.begin(baud);
#else
    _err = AARUARDUINO_UNSUPPORTED;
#endif
    return _err;
}

void AaruUARTEngine::setBufferSize(int sz) {}

int AaruUARTEngine::available() {
#if defined(ARDUINO)
    Stream* s = getStream();
    if (s) return s->available();
#endif
    return 0;
}
int AaruUARTEngine::peek() {
#if defined(ARDUINO)
    Stream* s = getStream();
    if (s) return s->peek();
#endif
    return -1;
}
int AaruUARTEngine::read() {
#if defined(ARDUINO)
    Stream* s = getStream();
    if (s) return s->read();
#endif
    return -1;
}
size_t AaruUARTEngine::readBytes(uint8_t* buf, size_t len) {
#if defined(ARDUINO)
    Stream* s = getStream();
    if (s) return s->readBytes(buf, len);
#endif
    return 0;
}
size_t AaruUARTEngine::readBytesUntil(char terminator, uint8_t* buf, size_t len) {
#if defined(ARDUINO)
    Stream* s = getStream();
    if (s) return s->readBytesUntil(terminator, buf, len);
#endif
    return 0;
}
size_t AaruUARTEngine::write(uint8_t b) {
#if defined(ARDUINO)
    Stream* s = getStream();
    if (s) return s->write(b);
#endif
    return 0;
}
size_t AaruUARTEngine::write(const uint8_t* buf, size_t len) {
#if defined(ARDUINO)
    Stream* s = getStream();
    if (s) return s->write(buf, len);
#endif
    return 0;
}
void AaruUARTEngine::flush() {
#if defined(ARDUINO)
    Stream* s = getStream();
    if (s) s->flush();
#endif
}

bool AaruUARTEngine::readLine(char* buf, size_t maxlen) {
    size_t i = 0;
    while (i < maxlen - 1) {
        int c = read();
        if (c < 0) return false;
        if (c == '\n') { buf[i] = 0; return true; }
        if (c != '\r') buf[i++] = (char)c;
    }
    buf[i] = 0;
    return true;
}

// Validate the trailing *HH checksum of an NMEA 0183 sentence.
bool AaruUARTEngine::nmeaValidate(const char* sentence) {
    if (!sentence || sentence[0] != '$') return false;
    const char* star = strchr(sentence, '*');
    if (!star || strlen(star) < 3) return false;

    uint8_t sum = 0;
    for (const char* p = sentence + 1; *p && *p != '*'; p++) {
        sum ^= (uint8_t)(*p);
    }

    char hex[3] = {star[1], star[2], '\0'};
    uint32_t given = (uint32_t)strtoul(hex, nullptr, 16);
    return (uint8_t)given == sum;
}

void AaruUARTEngine::setTimeout(uint16_t ms) {
    _timeout_ms = ms;
#if defined(ARDUINO)
    Stream* s = getStream();
    if (s) s->setTimeout(ms);
#endif
}

bool AaruUARTEngine::isBegun() const { return _begun; }
bool AaruUARTEngine::isHardware() const { return _hw; }
int AaruUARTEngine::getUartNum() const { return _uart_num; }
AaruarduinoError AaruUARTEngine::lastError() const { return _err; }
const char* AaruUARTEngine::errorString() const { return aaruarduinoErrorString(_err); }

Stream* AaruUARTEngine::getStream() {
#if defined(AARUARDUINO_HAS_HARDWARE_SERIAL)
    if (_hw_serial) return _hw_serial;
    return &Serial;
#elif defined(AARUARDUINO_HAS_SOFTWARE_SERIAL)
    if (_soft) return _soft;
    return &Serial;
#else
    return nullptr;
#endif
}
