// ============================================================
//  AARUARDUINO Analog Engine
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_ANALOG_ENGINE_H
#define AARUARDUINO_ANALOG_ENGINE_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"
#include "utility/AaruarduinoMath.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

typedef enum {
    AARU_AN_KIND_NONE           = 0,
    AARU_AN_KIND_RAW            = 1,
    AARU_AN_KIND_LDR            = 2,
    AARU_AN_KIND_UV_INDEX       = 5,
    AARU_AN_KIND_LM35           = 10,
    AARU_AN_KIND_TMP36          = 11,
    AARU_AN_KIND_NTC_BETA       = 12,
    AARU_AN_KIND_NTC_STEINHART  = 13,
    AARU_AN_KIND_THERMISTOR_10K = 14,
    AARU_AN_KIND_MQ2            = 20,
    AARU_AN_KIND_MQ3            = 21,
    AARU_AN_KIND_MQ4            = 22,
    AARU_AN_KIND_MQ5            = 23,
    AARU_AN_KIND_MQ6            = 24,
    AARU_AN_KIND_MQ7            = 25,
    AARU_AN_KIND_MQ8            = 26,
    AARU_AN_KIND_MQ9            = 27,
    AARU_AN_KIND_MQ131          = 28,
    AARU_AN_KIND_MQ135          = 29,
    AARU_AN_KIND_MQ136          = 30,
    AARU_AN_KIND_MQ137          = 31,
    AARU_AN_KIND_MQ138          = 32,
    AARU_AN_KIND_MQ214          = 33,
    AARU_AN_KIND_MQ303A         = 34,
    AARU_AN_KIND_MQ306A         = 35,
    AARU_AN_KIND_MQ307A         = 36,
    AARU_AN_KIND_MQ309A         = 37,
    AARU_AN_KIND_MQ403          = 38,
    AARU_AN_KIND_MQ501          = 39,
    AARU_AN_KIND_MQ601          = 40,
    AARU_AN_KIND_MQ701          = 41,
    AARU_AN_KIND_MQ801          = 42,
    AARU_AN_KIND_MQ901          = 43,
    AARU_AN_KIND_GENERIC_MQ     = 44,
    AARU_AN_KIND_MICS_5524      = 60,
    AARU_AN_KIND_MICS_6814      = 61,
    AARU_AN_KIND_ZE08_CH2O      = 62,
    AARU_AN_KIND_ZE25_O3        = 63,
    AARU_AN_KIND_ZE27_O3        = 64,
    AARU_AN_KIND_ZE03           = 65,
    AARU_AN_KIND_SOIL_MOISTURE  = 70,
    AARU_AN_KIND_SOIL_MOISTURE_CAP = 71,
    AARU_AN_KIND_RAIN_SENSOR    = 72,
    AARU_AN_KIND_WATER_LEVEL    = 73,
    AARU_AN_KIND_WATER_QUALITY  = 74,
    AARU_AN_KIND_SOUND_SENSOR   = 80,
    AARU_AN_KIND_VIBRATION_SW420 = 81,
    AARU_AN_KIND_PIEZO_VIBRATION = 82,
    AARU_AN_KIND_FSR            = 90,
    AARU_AN_KIND_PRESSURE_0_5V  = 92,
    AARU_AN_KIND_PRESSURE_4_20MA = 93,
    AARU_AN_KIND_ACS712_5A      = 100,
    AARU_AN_KIND_ACS712_20A     = 101,
    AARU_AN_KIND_ACS712_30A     = 102,
    AARU_AN_KIND_ACS758         = 103,
    AARU_AN_KIND_VOLTAGE_DIVIDER = 105,
    AARU_AN_KIND_AC_VOLTAGE_ZMPT101B = 106,
    AARU_AN_KIND_POTENTIOMETER  = 110,
    AARU_AN_KIND_SLIDE_POT      = 111,
    AARU_AN_KIND_ANGLE_SENSOR   = 112,
    AARU_AN_KIND_HALL_ANALOG    = 120,
    AARU_AN_KIND_LINEAR_HALL    = 121,
    AARU_AN_KIND_PULSE_SENSOR   = 130,
    AARU_AN_KIND_GSR            = 131,
    AARU_AN_KIND_EMG            = 132,
    AARU_AN_KIND_FLAME_ANALOG   = 140,
    AARU_AN_KIND_GAS_GENERIC    = 141,
    AARU_AN_KIND_DUST_GP2Y10    = 142,
    AARU_AN_KIND_DUST_GP2Y1010  = 143,
    AARU_AN_KIND_PH_ANALOG      = 147,
    AARU_AN_KIND_EC_ANALOG      = 148,
    AARU_AN_KIND_ORP_ANALOG     = 149,
    AARU_AN_KIND_CO2_MHZ19_ANALOG = 150,
} AaruAnalogKind;

struct AaruAnalogPreset {
    AaruAnalogKind kind;
    float param1;
    float param2;
    float param3;
    float param4;
    const char* name;
    const char* unit;
};

class AaruAnalogEngine {
public:
    AaruAnalogEngine() = default;

    AaruarduinoError begin(uint8_t pin, AaruAnalogKind kind);
    AaruarduinoError beginWithPreset(uint8_t pin, AaruAnalogKind kind,
                                     float p1 = 0, float p2 = 0, float p3 = 0, float p4 = 0);

    int readRaw();
    float readPercent();
    float readVoltage(float divider_ratio = 1.0f);
    float readValue();

    uint8_t getPin() const;
    AaruAnalogKind getKind() const;
    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;
    const char* kindName() const;
    const char* kindUnit() const;

    void setParams(float p1 = 0, float p2 = 0, float p3 = 0, float p4 = 0);
    void setDividerRatio(float ratio);
    void setSamples(uint8_t samples);
    void setSettleUs(uint16_t us);

    int readRawPin(uint8_t pin);
    float readVoltagePin(uint8_t pin, float divider_ratio = 1.0f);

private:
    uint8_t _pin;
    AaruAnalogKind _kind;
    AaruarduinoError _err;
    float _p1, _p2, _p3, _p4;
    float _divider;
    uint8_t _samples;
    uint16_t _settle_us;
    int _adc_max;
    float _vref;

    float convertRawToValue(int raw);
    const struct AaruAnalogPreset* findPreset(AaruAnalogKind kind) const;
    const char* presetName(AaruAnalogKind kind) const;
    const char* presetUnit(AaruAnalogKind kind) const;
};

#endif // AARUARDUINO_ANALOG_ENGINE_H
