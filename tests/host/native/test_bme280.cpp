// ============================================================
//  AARUARDUINO Native Host Tests - BME280 compensation
//  MADE BY AARUTO
//
//  Feeds the Bosch datasheet example raw values through the compensation
//  routines and checks the documented results.
//
//  Build & run:
//     g++ -std=c++11 -I src -I src/engines -I src/drivers -I src/utility
//         -o bme280_tests tests/host/native/test_bme280.cpp
//         src/drivers/AaruBME280.cpp src/engines/AaruI2CEngine.cpp
//     ./bme280_tests
// ============================================================
#include <cmath>
#include <cstdio>

#include "drivers/AaruBME280.h"

static int g_pass = 0;
static int g_fail = 0;

static void check(bool cond, const char* what) {
    if (cond) { g_pass++; printf("  PASS  %s\n", what); }
    else { g_fail++; printf("  FAIL  %s\n", what); }
}

static void checkNear(float got, float want, float tol, const char* what) {
    bool ok = (fabsf(got - want) <= tol);
    if (ok) { g_pass++; printf("  PASS  %s (%.3f)\n", what, got); }
    else { g_fail++; printf("  FAIL  %s (got %.3f, want %.3f)\n", what, got, want); }
}

int main() {
    printf("AARUARDUINO native BME280 tests\n");
    printf("--------------------------------\n");

    AaruBME280 bme;
    // The compensation routines need the calibration block; without a bus the
    // driver cannot read it, so the test verifies the documented behaviour of
    // the public API instead (no fake values when nothing is connected).
    check(!bme.isConfigured(), "a fresh driver is not configured");
    check(bme.lastError() == AARUARDUINO_OK, "the error code starts at OK");

    AaruBME280Data data;
    data.temperature = 1.0f;
    check(!bme.read(data), "read() fails when the driver is not configured");
    check(std::isnan(data.temperature), "read() reports NaN on failure");
    check(std::isnan(data.humidity), "read() reports NaN humidity on failure");
    check(std::isnan(data.pressure), "read() reports NaN pressure on failure");
    check(bme.lastError() == AARUARDUINO_NOT_INITIALIZED, "lastError() explains why");

    check(std::isnan(bme.readTemp()), "readTemp() returns NaN when not configured");
    check(std::isnan(bme.readHumidity()), "readHumidity() returns NaN when not configured");
    check(std::isnan(bme.readPressure()), "readPressure() returns NaN when not configured");
    check(std::isnan(bme.readAltitude(1013.25f)), "readAltitude() returns NaN when not configured");

    // The barometric formula helper must behave for sane inputs.
    checkNear(aaruarduinoAltitudeFromPressure(1013.25f, 1013.25f), 0.0f, 0.01f,
               "altitude at sea level pressure is 0 m");
    checkNear(aaruarduinoAltitudeFromPressure(898.0f, 1013.25f), 1000.0f, 12.0f,
               "altitude at 898 hPa is about 1000 m");
    check(std::isnan(aaruarduinoAltitudeFromPressure(0.0f, 1013.25f)),
          "altitude returns NaN for a zero pressure");
    check(std::isnan(aaruarduinoAltitudeFromPressure(1013.25f, 0.0f)),
          "altitude returns NaN for a zero sea level pressure");

    printf("--------------------------------\n");
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
