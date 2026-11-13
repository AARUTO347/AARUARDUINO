// ============================================================
//  AARUARDUINO DS18B20 Driver
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_DS18B20_H
#define AARUARDUINO_DS18B20_H

#include "AARUARDUINOTypes.h"
#include "engines/AaruOneWireEngine.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

class AaruDS18B20 {
public:
    AaruDS18B20() = default;

    AaruarduinoError begin(uint8_t pin);
    AaruarduinoError begin(AaruOneWireEngine* ow, uint8_t pin = 255);

    int discover();
    float readTempC(int index = 0);
    float readTempC(const AaruOneWireROM* rom);
    float readTempF(int index = 0);

    AaruarduinoError requestTemperatures();
    bool conversionReady();
    float readTempCAsync(int index);

    AaruarduinoError setResolution(int index, uint8_t bits);
    AaruarduinoError setResolution(const AaruOneWireROM* rom, uint8_t bits);

    int alarmSearch(AaruOneWireROM* roms, int max);

    int deviceCount() const;
    const AaruOneWireROM* getROM(int index) const;
    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured = false;
    AaruOneWireEngine* _ow = nullptr;
    bool _own_ow = false;
    AaruOneWireROM _roms[16] = {};
    int _count = 0;
    unsigned long _last_convert = 0;
    AaruarduinoError _err = AARUARDUINO_OK;
};

#endif // AARUARDUINO_DS18B20_H
