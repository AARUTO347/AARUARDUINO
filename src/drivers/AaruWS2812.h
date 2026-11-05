// ============================================================
//  AARUARDUINO WS2812 (NeoPixel) Driver
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_WS2812_H
#define AARUARDUINO_WS2812_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

struct AaruRGB {
    uint8_t r, g, b;
    AaruRGB(uint8_t r_=0, uint8_t g_=0, uint8_t b_=0) : r(r_), g(g_), b(b_) {}
};

class AaruWS2812 {
public:
    AaruWS2812() = default;

    AaruarduinoError begin(uint8_t pin, uint16_t num_leds);
    AaruarduinoError begin(uint8_t pin, uint16_t num_leds, uint8_t type);

    AaruarduinoError setPixel(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
    AaruarduinoError setPixel(uint16_t index, struct AaruRGB color);
    AaruarduinoError setPixelHSV(uint16_t index, uint16_t h, uint8_t s, uint8_t v);
    AaruarduinoError fill(uint8_t r, uint8_t g, uint8_t b, uint16_t start, uint16_t len);
    AaruarduinoError fill(struct AaruRGB color, uint16_t start, uint16_t len);
    AaruarduinoError clear();
    AaruarduinoError show();

    void setBrightness(uint8_t b);

    AaruarduinoError rainbow(uint16_t first_hue, uint16_t hue_step);
    AaruarduinoError theaterChase(uint8_t r, uint8_t g, uint8_t b, uint8_t wait_ms);
    AaruarduinoError colorWipe(uint8_t r, uint8_t g, uint8_t b, uint8_t wait_ms);

    uint16_t numLeds() const;
    struct AaruRGB getPixel(uint16_t index) const;
    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured;
    uint8_t _pin;
    uint16_t _num_leds;
    uint8_t _type;
    uint8_t _brightness;
    struct AaruRGB* _pixels;
    AaruarduinoError _err;

    void sendBytes(const uint8_t* data, uint32_t len);
    static uint32_t rgbToRaw(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
};

#endif // AARUARDUINO_WS2812_H
