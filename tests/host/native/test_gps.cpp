// ============================================================
//  AARUARDUINO Native Host Tests - GPS / NMEA parser
//  MADE BY AARUTO
//
//  Builds the real driver sources for the host (no Arduino toolchain needed)
//  and verifies the NMEA 0183 parser end to end.
//
//  Build & run (MinGW / GCC on Windows, or any POSIX shell):
//     g++ -std=c++11 -I src -I src/engines -o gps_tests
//         tests/host/native/test_gps.cpp
//         src/drivers/AaruGPS.cpp src/engines/AaruUARTEngine.cpp
//     ./gps_tests
// ============================================================
#include <cmath>
#include <cstdio>
#include <cstring>

#include "drivers/AaruGPS.h"

static int g_pass = 0;
static int g_fail = 0;

static void check(bool cond, const char* what) {
    if (cond) {
        g_pass++;
        printf("  PASS  %s\n", what);
    } else {
        g_fail++;
        printf("  FAIL  %s\n", what);
    }
}

static void checkNear(float got, float want, float tol, const char* what) {
    bool ok = (fabsf(got - want) <= tol);
    if (ok) {
        g_pass++;
        printf("  PASS  %s (%.4f)\n", what, got);
    } else {
        g_fail++;
        printf("  FAIL  %s (got %.4f, want %.4f)\n", what, got, want);
    }
}

int main() {
    printf("AARUARDUINO native GPS tests\n");
    printf("----------------------------\n");

    // ---- 1. GGA: fix position, satellites, altitude ----
    {
        AaruGPS gps;
        bool ok = gps.feedSentence(
            "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
        check(ok, "GGA sentence accepted");
        check(gps.hasFix(), "GGA reports a valid fix");
        checkNear(gps.lat(), 48.117300f, 0.0005f, "GGA latitude (deg)");
        checkNear(gps.lon(), 11.516667f, 0.0005f, "GGA longitude (deg)");
        checkNear(gps.alt(), 545.4f, 0.05f, "GGA altitude (m)");
        check(gps.sats() == 8, "GGA satellites used = 8");
        check(gps.time() == 123519UL, "GGA UTC time = 123519");
    }

    // ---- 2. Southern / Western hemisphere sign handling ----
    {
        AaruGPS gps;
        gps.feedSentence("$GPGGA,123519,4807.038,S,01131.000,W,1,08,0.9,545.4,M,46.9,M,,*48");
        check(gps.lat() < 0.0f, "southern latitude is negative");
        check(gps.lon() < 0.0f, "western longitude is negative");
        checkNear(gps.lat(), -48.117300f, 0.0005f, "S latitude value");
        checkNear(gps.lon(), -11.516667f, 0.0005f, "W longitude value");
    }

    // ---- 3. RMC: speed, course, date ----
    {
        AaruGPS gps;
        bool ok = gps.feedSentence("$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A");
        check(ok, "RMC sentence accepted");
        checkNear(gps.speedKnots(), 22.4f, 0.05f, "RMC speed (knots)");
        checkNear(gps.speedKmph(), 41.4848f, 0.01f, "RMC speed (km/h)");
        checkNear(gps.course(), 84.4f, 0.05f, "RMC course (deg)");
        check(gps.date() == 19940323UL, "RMC date = 1994-03-23");
        check(gps.time() == 123519UL, "RMC UTC time");
    }

    // ---- 4. Invalid fix (quality 0 / status V) must not report a fix ----
    {
        AaruGPS gps;
        gps.feedSentence("$GPGGA,123519,4807.038,N,01131.000,E,0,00,,,M,,M,,*52");
        check(!gps.hasFix(), "GGA quality 0 -> no fix");

        AaruGPS gps2;
        gps2.feedSentence("$GPRMC,123519,V,4807.038,N,01131.000,E,,,230394,,,N*68");
        check(!gps2.hasFix(), "RMC status V -> no fix");
    }

    // ---- 5. GSV: satellite list ----
    {
        AaruGPS gps;
        bool ok = gps.feedSentence(
            "$GPGSV,2,1,08,01,040,083,02,17,308,41,12,07,344,39,14,22,228,45*6B");
        check(ok, "GSV sentence accepted");
        check(gps.satCount() == 4, "GSV parsed 4 satellites");
        const AaruGPSSatellite* s = gps.sat(0);
        check(s != nullptr, "satellite 0 available");
        if (s) {
            check(s->prn == 8, "satellite 0 PRN = 8");
            check(s->elevation == 1, "satellite 0 elevation = 1");
            check(s->azimuth == 40, "satellite 0 azimuth = 40");
            check(s->snr == 83, "satellite 0 SNR = 83");
        }
        const AaruGPSSatellite* s1 = gps.sat(1);
        if (s1) {
            check(s1->prn == 2, "satellite 1 PRN = 2");
            check(s1->elevation == 17, "satellite 1 elevation = 17");
            check(s1->azimuth == 308, "satellite 1 azimuth = 308");
            check(s1->snr == 41, "satellite 1 SNR = 41");
        }
        check(gps.sat(99) == nullptr, "out-of-range satellite index returns null");
    }

    // ---- 6. VTG: course over ground ----
    {
        AaruGPS gps;
        bool ok = gps.feedSentence("$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48");
        check(ok, "VTG sentence accepted");
        checkNear(gps.course(), 54.7f, 0.05f, "VTG true track");
        checkNear(gps.speedKnots(), 5.5f, 0.05f, "VTG speed (knots)");
    }

    // ---- 7. Bad checksum must be rejected ----
    {
        AaruGPS gps;
        bool ok = gps.feedSentence("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*00");
        check(!ok, "corrupted checksum rejected");
        check(!gps.hasFix(), "corrupted sentence does not set a fix");
        check(gps.lastError() == AARUARDUINO_CRC_ERROR, "CRC error reported");
    }

    // ---- 8. Unknown talker is ignored ----
    {
        AaruGPS gps;
        bool ok = gps.feedSentence("$XXGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*50");
        check(!ok, "unknown talker ignored");
    }

    printf("----------------------------\n");
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
