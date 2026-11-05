// ============================================================
//  AARUARDUINO WS2812 Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruWS2812.h"


AaruarduinoError AaruWS2812::begin(uint8_t pin, uint16_t num_leds) {
    return begin(pin, num_leds, 0);
}
AaruarduinoError AaruWS2812::begin(uint8_t pin, uint16_t num_leds, uint8_t type) {
    _pin = pin; _num_leds = num_leds; _type = type; _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    _pixels = new AaruRGB[num_leds];
    memset(_pixels, 0, num_leds * sizeof(AaruRGB));
#endif
    return _err;
}

AaruarduinoError AaruWS2812::setPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (index < _num_leds) _pixels[index] = AaruRGB(r, g, b);
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruWS2812::setPixel(uint16_t index, AaruRGB color) {
    return setPixel(index, color.r, color.g, color.b);
}
AaruarduinoError AaruWS2812::setPixelHSV(uint16_t index, uint16_t h, uint8_t s, uint8_t v) {
    _err = AARUARDUINO_OK; return _err;
}

AaruarduinoError AaruWS2812::fill(uint8_t r, uint8_t g, uint8_t b, uint16_t start, uint16_t len) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (len == 0xFFFF) len = _num_leds;
    for (uint16_t i = start; i < start + len && i < _num_leds; i++) {
        _pixels[i] = AaruRGB(r, g, b);
    }
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruWS2812::fill(AaruRGB color, uint16_t start, uint16_t len) {
    return fill(color.r, color.g, color.b, start, len);
}
AaruarduinoError AaruWS2812::clear() {
    return fill(0, 0, 0, 0, _num_leds);
}
AaruarduinoError AaruWS2812::show() {
    _err = AARUARDUINO_OK; return _err;
}

void AaruWS2812::setBrightness(uint8_t b) { _brightness = b; }

AaruarduinoError AaruWS2812::rainbow(uint16_t first_hue, uint16_t hue_step) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruWS2812::theaterChase(uint8_t r, uint8_t g, uint8_t b, uint8_t wait_ms) {
    _err = AARUARDUINO_OK; return _err;
}
AaruarduinoError AaruWS2812::colorWipe(uint8_t r, uint8_t g, uint8_t b, uint8_t wait_ms) {
    _err = AARUARDUINO_OK; return _err;
}

uint16_t AaruWS2812::numLeds() const { return _num_leds; }
AaruRGB AaruWS2812::getPixel(uint16_t index) const {
    if (index < _num_leds) return _pixels[index];
    return AaruRGB(0, 0, 0);
}
bool AaruWS2812::isConfigured() const { return _configured; }
AaruarduinoError AaruWS2812::lastError() const { return _err; }
const char* AaruWS2812::errorString() const { return aaruarduinoErrorString(_err); }

void AaruWS2812::sendBytes(const uint8_t* data, uint32_t len) {}
uint32_t AaruWS2812::rgbToRaw(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) { return 0; }
