// ============================================================
//  AARUARDUINO GPS (NMEA) Driver
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_GPS_H
#define AARUARDUINO_GPS_H

#include "AARUARDUINOTypes.h"
#include "engines/AaruUARTEngine.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

struct AaruGPSFix {
    bool valid;
    float latitude;
    float longitude;
    float altitude;
    float speed;
    float course;
    float hdop;
    uint8_t satellites;
    uint8_t fix_quality;
    uint32_t date;
    uint32_t time;
    uint32_t updated;
};

struct AaruGPSSatellite {
    uint8_t prn;
    uint8_t elevation;
    uint16_t azimuth;
    uint8_t snr;
    bool used;
};

class AaruGPS {
public:
    AaruGPS() = default;

    AaruarduinoError begin(class AaruUARTEngine* uart);
    AaruarduinoError begin(int rx, int tx, uint32_t baud);

    void update();

    const struct AaruGPSFix& fix() const;
    bool hasFix() const;
    float lat() const;
    float lon() const;
    float alt() const;
    float speedKnots() const;
    float speedKmph() const;
    float course() const;
    uint8_t sats() const;
    uint32_t date() const;
    uint32_t time() const;

    int satCount() const;
    const struct AaruGPSSatellite* sat(int idx) const;

    void onGGA(void (*cb)(const struct AaruGPSFix&));
    void onRMC(void (*cb)(const struct AaruGPSFix&));
    void onGSV(void (*cb)(const struct AaruGPSSatellite*, int));
    void onRaw(void (*cb)(const char*));

    AaruarduinoError setRateHz(uint8_t hz);
    AaruarduinoError setBaud(uint32_t baud);
    AaruarduinoError enableNMEA(uint8_t mask);
    AaruarduinoError sleep();
    AaruarduinoError wake();

    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured;
    class AaruUARTEngine* _uart;
    bool _own_uart;
    struct AaruGPSFix _fix;
    struct AaruGPSSatellite _sats[16];
    int _sat_count;
    char _buf[128];
    int _buf_idx;
    AaruarduinoError _err;

    void (*_cb_gga)(const struct AaruGPSFix&);
    void (*_cb_rmc)(const struct AaruGPSFix&);
    void (*_cb_gsv)(const struct AaruGPSSatellite*, int);
    void (*_cb_raw)(const char*);

    void parseSentence(const char* sentence);
    bool parseGGA(const char* s);
    bool parseRMC(const char* s);
    bool parseGSV(const char* s);
    bool parseVTG(const char* s);
    static uint8_t nmeaChecksum(const char* s);
};

#endif // AARUARDUINO_GPS_H
