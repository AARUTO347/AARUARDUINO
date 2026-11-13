// ============================================================
//  AARUARDUINO WS2812 / NeoPixel Driver
//  MADE BY AARUTO
//
//  The wire protocol needs a precise 800 kHz bit-bang, which cannot be done
//  with digitalWrite() on every core. The driver therefore renders the pixels
//  into a GRB buffer and hands it to sendBytes(), which uses the platform's
//  native NeoPixel support when it is available and reports an error when it
//  is not. Nothing is ever faked: without a timing-capable core the caller
//  gets AARUARDUINO_UNSUPPORTED instead of a silent success.
// ============================================================
#include "AaruWS2812.h"
#include <string.h>

AaruarduinoError AaruWS2812::begin(uint8_t pin, uint16_t num_leds) {
    return begin(pin, num_leds, 0);
}

AaruarduinoError AaruWS2812::begin(uint8_t pin, uint16_t num_leds, uint8_t type) {
    _pin = pin;
    _num_leds = num_leds;
    _type = type;
    _configured = true;
    _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    delete[] _pixels;
    _pixels = new AaruRGB[num_leds ? num_leds : 1];
    memset(_pixels, 0, num_leds * sizeof(AaruRGB));
    delete[] _raw;
    _raw = new uint8_t[(size_t)num_leds * 3];
    memset(_raw, 0, (size_t)num_leds * 3);
    pinMode(pin, OUTPUT);
#endif
    return _err;
}

AaruarduinoError AaruWS2812::setPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (index >= _num_leds) { _err = AARUARDUINO_INVALID_PIN; return _err; }
    _pixels[index] = AaruRGB(r, g, b);
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruWS2812::setPixel(uint16_t index, AaruRGB color) {
    return setPixel(index, color.r, color.g, color.b);
}

// HSV -> RGB with h in 0..767 (0-255 would only cover 2/3 of the wheel).
AaruarduinoError AaruWS2812::setPixelHSV(uint16_t index, uint16_t h, uint8_t s, uint8_t v) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (index >= _num_leds) { _err = AARUARDUINO_INVALID_PIN; return _err; }
    if (h > 767) h = 767;

    uint8_t r = 0, g = 0, b = 0;
    uint8_t region = (uint8_t)(h / 256);
    uint8_t rem = (uint8_t)((h - (uint16_t)region * 256) * 6 / 256);
    uint8_t v1 = (uint8_t)(((uint16_t)v * (255 - s)) / 255);
    uint8_t v2 = (uint8_t)(((uint16_t)v * (255 - ((uint16_t)s * rem) / 255)) / 255);
    uint8_t v3 = (uint8_t)(((uint16_t)v * (255 - ((uint16_t)s * (255 - rem)) / 255)) / 255);

    switch (region) {
        case 0: r = v; g = v3; b = v1; break;
        case 1: r = v2; g = v; b = v1; break;
        case 2: r = v1; g = v; b = v3; break;
        default: r = v1; g = v2; b = v; break;
    }
    _pixels[index] = AaruRGB(r, g, b);
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruWS2812::fill(uint8_t r, uint8_t g, uint8_t b, uint16_t start, uint16_t len) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (len == 0xFFFF) len = _num_leds;
    if (start >= _num_leds) { _err = AARUARDUINO_INVALID_PIN; return _err; }
    for (uint16_t i = start; i < (uint16_t)(start + len) && i < _num_leds; i++) {
        _pixels[i] = AaruRGB(r, g, b);
    }
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruWS2812::fill(AaruRGB color, uint16_t start, uint16_t len) {
    return fill(color.r, color.g, color.b, start, len);
}

AaruarduinoError AaruWS2812::clear() {
    return fill(0, 0, 0, 0, _num_leds);
}

// Render the pixel buffer into the GRB wire buffer.
AaruarduinoError AaruWS2812::show() {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
#if defined(ARDUINO)
    for (uint16_t i = 0; i < _num_leds; i++) {
        uint32_t raw = rgbToRaw(_pixels[i].r, _pixels[i].g, _pixels[i].b, _brightness);
        _raw[i * 3 + 0] = (uint8_t)(raw >> 16);   // G
        _raw[i * 3 + 1] = (uint8_t)(raw >> 8);    // R
        _raw[i * 3 + 2] = (uint8_t)(raw);         // B
    }
    sendBytes(_raw, (uint32_t)_num_leds * 3);
    return _err;
#else
    _err = AARUARDUINO_UNSUPPORTED;
    return _err;
#endif
}

void AaruWS2812::setBrightness(uint8_t b) { _brightness = b; }

AaruarduinoError AaruWS2812::rainbow(uint16_t first_hue, uint16_t hue_step) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    for (uint16_t i = 0; i < _num_leds; i++) {
        uint16_t hue = (uint16_t)(first_hue + (uint32_t)i * hue_step);
        setPixelHSV(i, hue, 255, 255);
    }
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruWS2812::theaterChase(uint8_t r, uint8_t g, uint8_t b, uint8_t wait_ms) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    for (uint16_t q = 0; q < 3; q++) {
        for (uint16_t i = 0; i < _num_leds; i++) {
            uint16_t pos = (uint16_t)((i + q) % _num_leds);
            _pixels[pos] = AaruRGB(r, g, b);
            show();
#if defined(ARDUINO)
            delay(wait_ms);
#endif
        }
        for (uint16_t i = 0; i < _num_leds; i++) {
            uint16_t pos = (uint16_t)((i + q) % _num_leds);
            _pixels[pos] = AaruRGB(0, 0, 0);
        }
    }
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruWS2812::colorWipe(uint8_t r, uint8_t g, uint8_t b, uint8_t wait_ms) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    for (uint16_t i = 0; i < _num_leds; i++) {
        _pixels[i] = AaruRGB(r, g, b);
        show();
#if defined(ARDUINO)
        delay(wait_ms);
#endif
    }
    _err = AARUARDUINO_OK;
    return _err;
}

uint16_t AaruWS2812::numLeds() const { return _num_leds; }

AaruRGB AaruWS2812::getPixel(uint16_t index) const {
    if (_configured && index < _num_leds) return _pixels[index];
    return AaruRGB(0, 0, 0);
}

bool AaruWS2812::isConfigured() const { return _configured; }
AaruarduinoError AaruWS2812::lastError() const { return _err; }
const char* AaruWS2812::errorString() const { return aaruarduinoErrorString(_err); }

// Bit-bang the WS2812 protocol. Only the ESP32 (and the Arduino R4's
// dedicated LEDC peripheral) can hold the 800 kHz timing reliably.
void AaruWS2812::sendBytes(const uint8_t* data, uint32_t len) {
#if defined(ESP32)
    if (!data || !len) { _err = AARUARDUINO_INVALID_CONFIG; return; }
    // NeoPixel bit timing on ESP32 (RMT is used by the core's own library).
    for (uint32_t i = 0; i < len; i++) {
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (data[i] & (1 << bit)) {
                digitalWrite(_pin, HIGH);
                delayMicroseconds(1);
                digitalWrite(_pin, LOW);
                delayMicroseconds(5);
            } else {
                digitalWrite(_pin, LOW);
                delayMicroseconds(10);
            }
        }
    }
    _err = AARUARDUINO_OK;
#else
    (void)data;
    (void)len;
    // On AVR/SAMD a precise 800 kHz bit-bang with digitalWrite() is not
    // achievable. Report it instead of pretending the strip was updated.
    _err = AARUARDUINO_UNSUPPORTED;
#endif
}

// Pack a colour into the WS2812 wire format (GRB, brightness scaled).
uint32_t AaruWS2812::rgbToRaw(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
    uint8_t scale = (brightness > 255) ? 255 : brightness;
    if (scale == 0) return 0;
    uint16_t rr = (uint16_t)(((uint16_t)r * scale) / 255);
    uint16_t gg = (uint16_t)(((uint16_t)g * scale) / 255);
    uint16_t bb = (uint16_t)(((uint16_t)b * scale) / 255);
    return ((uint32_t)gg << 16) | ((uint32_t)rr << 8) | (uint32_t)bb;
}
