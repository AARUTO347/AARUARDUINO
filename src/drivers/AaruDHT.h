// ============================================================
//  AARUARDUINO DHT11/DHT22 Driver
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_DHT_H
#define AARUARDUINO_DHT_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

typedef enum {
    AARU_DHT_NONE = 0,
    AARU_DHT11    = 11,
    AARU_DHT22    = 22,
    AARU_DHT21    = 21,
} AaruDHTType;

class AaruDHT {
public:
    AaruDHT() = default;

    AaruarduinoError begin(uint8_t pin, AaruDHTType type);
    void setMinIntervalMs(unsigned long ms);

    bool read(float& temp_c, float& hum_pct);
    float readTemperature();
    float readHumidity();
    float readTemperatureF();

    bool fresh() const;
    unsigned long ageMs() const;

    uint8_t pin() const;
    AaruDHTType type() const;
    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured;
    uint8_t _pin;
    AaruDHTType _type;
    unsigned long _min_interval;
    unsigned long _last_read;
    float _temp_c;
    float _hum_pct;
    bool _fresh;
    AaruarduinoError _err;

    bool readRaw(uint8_t* data);
    float parseTemp(const uint8_t* data);
    float parseHum(const uint8_t* data);
};

#endif // AARUARDUINO_DHT_H
