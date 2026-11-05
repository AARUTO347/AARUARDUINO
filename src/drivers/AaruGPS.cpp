// ============================================================
//  AARUARDUINO GPS Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruGPS.h"

AaruarduinoError AaruGPS::begin(AaruUARTEngine* uart) {
    _uart = uart; _configured = true; _err = AARUARDUINO_OK;
    return _err;
}
AaruarduinoError AaruGPS::begin(int rx, int tx, uint32_t baud) {
    _configured = true; _err = AARUARDUINO_OK;
    return _err;
}

void AaruGPS::update() {}

const AaruGPSSatellite* AaruGPS::sat(int idx) const {
    if (idx < _sat_count) return &_sats[idx];
    return nullptr;
}

void AaruGPS::onGGA(void (*cb)(const AaruGPSFix&)) { _cb_gga = cb; }
void AaruGPS::onRMC(void (*cb)(const AaruGPSFix&)) { _cb_rmc = cb; }
void AaruGPS::onGSV(void (*cb)(const AaruGPSSatellite*, int)) { _cb_gsv = cb; }
void AaruGPS::onRaw(void (*cb)(const char*)) { _cb_raw = cb; }

AaruarduinoError AaruGPS::setRateHz(uint8_t hz) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruGPS::setBaud(uint32_t baud) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruGPS::enableNMEA(uint8_t mask) { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruGPS::sleep() { _err = AARUARDUINO_OK; return _err; }
AaruarduinoError AaruGPS::wake() { _err = AARUARDUINO_OK; return _err; }

bool AaruGPS::isConfigured() const { return _configured; }
AaruarduinoError AaruGPS::lastError() const { return _err; }
const char* AaruGPS::errorString() const { return aaruarduinoErrorString(_err); }

void AaruGPS::parseSentence(const char* sentence) {}
bool AaruGPS::parseGGA(const char* s) { return false; }
bool AaruGPS::parseRMC(const char* s) { return false; }
bool AaruGPS::parseGSV(const char* s) { return false; }
bool AaruGPS::parseVTG(const char* s) { return false; }
uint8_t AaruGPS::nmeaChecksum(const char* s) { return 0; }
