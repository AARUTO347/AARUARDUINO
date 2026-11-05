// ============================================================
//  AARUARDUINO DS3231 RTC Driver
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_DS3231_H
#define AARUARDUINO_DS3231_H

#include "AARUARDUINOTypes.h"
#include "engines/AaruI2CEngine.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

#define DS3231_ADDR 0x68

struct AaruDateTime {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t dow;
};

class AaruDS3231 {
public:
    AaruDS3231() = default;

    AaruarduinoError begin(uint8_t addr);
    AaruarduinoError begin(class AaruI2CEngine* i2c, uint8_t addr);

    AaruarduinoError setDateTime(const AaruDateTime& dt);
    AaruarduinoError getDateTime(AaruDateTime& dt);
    AaruarduinoError setUnixTime(uint32_t epoch);
    uint32_t getUnixTime();

    float readTempC();

    AaruarduinoError setAlarm1(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, bool match_dow);
    AaruarduinoError setAlarm2(uint8_t min, uint8_t hour, uint8_t day, bool match_dow);
    AaruarduinoError clearAlarm(uint8_t alarm_num);
    bool alarmTriggered(uint8_t alarm_num);

    AaruarduinoError setSquareWave(uint8_t freq);
    AaruarduinoError setAgingOffset(int8_t offset);
    AaruarduinoError writeSRAM(uint8_t offset, const uint8_t* data, uint8_t len);
    AaruarduinoError readSRAM(uint8_t offset, uint8_t* data, uint8_t len);

    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured;
    uint8_t _addr;
    class AaruI2CEngine* _i2c;
    bool _own_i2c;
    AaruarduinoError _err;

    static uint8_t decToBcd(uint8_t v);
    static uint8_t bcdToDec(uint8_t v);
    AaruarduinoError writeReg(uint8_t reg, uint8_t val);
    AaruarduinoError readReg(uint8_t reg, uint8_t* val);
    AaruarduinoError readRegs(uint8_t reg, uint8_t* buf, uint8_t len);
};

#endif // AARUARDUINO_DS3231_H
