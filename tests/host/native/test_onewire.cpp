// ============================================================
//  AARUARDUINO Native Host Tests - 1-Wire (DS18B20)
//  MADE BY AARUTO
//
//  Verifies the Dallas CRC-8 and the DS18B20 scratchpad decoding, which are
//  the parts that can be tested without a bus.
//
//  Build & run:
//     g++ -std=c++11 -I src -I src/engines -I src/drivers
//         -o onewire_tests tests/host/native/test_onewire.cpp
//         src/engines/AaruOneWireEngine.cpp
//     ./onewire_tests
// ============================================================
#include <cstdio>
#include <cstring>

#include "engines/AaruOneWireEngine.h"

static int g_pass = 0;
static int g_fail = 0;

static void check(bool cond, const char* what) {
    if (cond) { g_pass++; printf("  PASS  %s\n", what); }
    else { g_fail++; printf("  FAIL  %s\n", what); }
}

// Reference CRC-8 (polynomial 0x31) for the well known DS18B20 ROM
// 28 FF 4C 5D 01 08 00 ... - the checksum is 0x7C.
int main() {
    printf("AARUARDUINO native 1-Wire tests\n");
    printf("------------------------------\n");

    const uint8_t rom[8] = {0x28, 0xFF, 0x4C, 0x5D, 0x01, 0x08, 0x00, 0x00};

    // Dallas/Maxim CRC-8 (polynomial 0x8C, reflected) reference values.
    uint8_t crc = AaruOneWireEngine::crc8(rom, 7);
    check(crc == 0x18, "CRC-8 of the DS18B20 ROM bytes is 0x18");

    const uint8_t rom2[8] = {0x28, 0x0D, 0x01, 0x08, 0x00, 0x00, 0x00, 0x00};
    uint8_t crc2 = AaruOneWireEngine::crc8(rom2, 7);
    check(crc2 == 0x85, "CRC-8 of the second ROM is 0x85");

    const uint8_t rom3[8] = {0x28, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};
    uint8_t crc3 = AaruOneWireEngine::crc8(rom3, 7);
    check(crc3 == 0x0C, "CRC-8 of the all-ones ROM is 0x0C");

    // A single bit flip must change the checksum.
    uint8_t rom4[7] = {0x28, 0xFF, 0x4C, 0x5D, 0x01, 0x08, 0x00};
    uint8_t crcA = AaruOneWireEngine::crc8(rom4, 7);
    rom4[3] ^= 0x01;
    uint8_t crcB = AaruOneWireEngine::crc8(rom4, 7);
    check(crcA != crcB, "a flipped bit changes the CRC");

    // checkCRC() must actually validate, not always return true.
    AaruOneWireROM r;
    r.family = rom2[0];
    memcpy(r.serial, rom2 + 1, 6);
    r.crc = crc2;
    check(AaruOneWireEngine::checkCRC(&r), "a correct ROM passes the CRC check");

    r.crc = (uint8_t)(crc2 ^ 0xFF);
    check(!AaruOneWireEngine::checkCRC(&r), "a corrupted ROM fails the CRC check");

    r.crc = 0x00;
    check(!AaruOneWireEngine::checkCRC(&r), "a zero CRC fails the check");
    check(!AaruOneWireEngine::checkCRC(nullptr), "a null ROM fails the check");

    printf("------------------------------\n");
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
