// ============================================================
//  AARUARDUINO UART Engine
//  MADE BY AARUTO
// ============================================================
#include "AaruUARTEngine.h"

AaruarduinoError AaruUARTEngine::beginHardware(int uart_num, uint32_t baud,
                                               int rx, int tx, uint8_t config) {
    _begun = true; _hw = true; _uart_num = uart_num; _baud = baud; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    if (rx >= 0 && tx >= 0) {
#if defined(ESP32)
        HardwareSerial* hs = new HardwareSerial(uart_num);
        hs->begin(baud, config, rx, tx);
        _hw_serial = hs;
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
#if defined(ARDUINO) && !defined(ESP32) && !defined(ESP8266)
    _soft = new SoftwareSerial(rx, tx, invert);
    _soft->begin(baud);
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

bool AaruUARTEngine::nmeaValidate(const char* sentence) {
    return true;
}

Stream* AaruUARTEngine::getStream() {
#if defined(ARDUINO) && defined(ESP32)
    if (_hw_serial) return _hw_serial;
    return &Serial;
#elif defined(ARDUINO) && !defined(ESP32) && !defined(ESP8266)
    if (_soft) return _soft;
    return &Serial;
#else
    return &Serial;
#endif
}
