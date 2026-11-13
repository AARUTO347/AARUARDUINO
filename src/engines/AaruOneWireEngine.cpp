// ============================================================
//  AARUARDUINO 1-Wire Engine
//  MADE BY AARUTO
//
//  Bit-banged Dallas/Maxim 1-Wire protocol (DS18B20 and compatible).
//  Timing follows the DS18B20 datasheet: 480 us reset pulse, 60/75 us
//  slot timings, 15 us recovery slot.
// ============================================================
#include "AaruOneWireEngine.h"

AaruarduinoError AaruOneWireEngine::begin(uint8_t pin) {
    _pin = pin;
    _begun = true;
    _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(pin, INPUT_PULLUP);
#endif
    return _err;
}

bool AaruOneWireEngine::reset() {
#if defined(ARDUINO)
    if (!_begun) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return false;
    }
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delayMicroseconds(480);
    pinMode(_pin, INPUT);
    delayMicroseconds(70);
    uint8_t r = digitalRead(_pin);
    delayMicroseconds(410);
    if (r != 0) {
        _err = AARUARDUINO_READ_FAIL;
        return false;
    }
    _err = AARUARDUINO_OK;
    return true;
#else
    _err = AARUARDUINO_UNSUPPORTED;
    return false;
#endif
}

bool AaruOneWireEngine::resetSearch() {
    // A search is restarted by issuing a reset pulse and clearing the
    // discrepancy state; the caller re-seeds the search state.
    return reset();
}

// --- low level bit primitives ---------------------------------------------

void AaruOneWireEngine::write1() {
#if defined(ARDUINO)
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delayMicroseconds(6);
    pinMode(_pin, INPUT);
    delayMicroseconds(64);
#else
    (void)this;
#endif
}

void AaruOneWireEngine::write0() {
#if defined(ARDUINO)
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delayMicroseconds(60);
    pinMode(_pin, INPUT);
    delayMicroseconds(10);
#else
    (void)this;
#endif
}

uint8_t AaruOneWireEngine::readBitRaw() {
#if defined(ARDUINO)
    pinMode(_pin, INPUT);
    delayMicroseconds(6);
    uint8_t bit = digitalRead(_pin);
    delayMicroseconds(64);
    return bit;
#else
    return 1;   // idle bus reads high
#endif
}

uint8_t AaruOneWireEngine::readBit() {
#if defined(ARDUINO)
    if (!_begun) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return 1;
    }
    uint8_t bit = readBitRaw();
    if (bit != 0 && bit != 1) {
        _err = AARUARDUINO_READ_FAIL;
        return 1;
    }
    _err = AARUARDUINO_OK;
    return bit;
#else
    _err = AARUARDUINO_UNSUPPORTED;
    return 1;
#endif
}

void AaruOneWireEngine::writeBit(uint8_t bit) {
#if defined(ARDUINO)
    if (!_begun) {
        _err = AARUARDUINO_NOT_INITIALIZED;
        return;
    }
    if (bit) write1(); else write0();
    _err = AARUARDUINO_OK;
#else
    (void)bit;
    _err = AARUARDUINO_UNSUPPORTED;
#endif
}

uint8_t AaruOneWireEngine::readByte() {
    uint8_t value = 0;
    for (uint8_t i = 0; i < 8; i++) {
        uint8_t bit = readBit();
        value >>= 1;
        value |= (bit & 1) << 7;   // 1-Wire sends the LSB first
    }
    return value;
}

void AaruOneWireEngine::writeByte(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
        writeBit(byte & 0x01);
        byte >>= 1;
    }
}

void AaruOneWireEngine::readBytes(uint8_t* buf, uint16_t len) {
    if (!buf) return;
    for (uint16_t i = 0; i < len; i++) buf[i] = readByte();
}

void AaruOneWireEngine::writeBytes(const uint8_t* buf, uint16_t len) {
    if (!buf) return;
    for (uint16_t i = 0; i < len; i++) writeByte(buf[i]);
}

// --- ROM commands ----------------------------------------------------------

AaruarduinoError AaruOneWireEngine::readROM(AaruOneWireROM* rom) {
    if (!rom) {
        _err = AARUARDUINO_INVALID_CONFIG;
        return _err;
    }
    if (!reset()) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }
    writeByte(0x33);            // Read ROM
    uint8_t raw[8];
    readBytes(raw, 8);

    rom->family = raw[0];
    for (uint8_t i = 0; i < 6; i++) rom->serial[i] = raw[1 + i];
    rom->crc = raw[7];

    _err = checkCRC(rom) ? AARUARDUINO_OK : AARUARDUINO_CRC_ERROR;
    return _err;
}

AaruarduinoError AaruOneWireEngine::matchROM(const AaruOneWireROM* rom) {
    if (!rom) {
        _err = AARUARDUINO_INVALID_CONFIG;
        return _err;
    }
    if (!reset()) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }
    writeByte(0x55);            // Match ROM
    writeByte(rom->family);
    writeBytes(rom->serial, 6);
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruOneWireEngine::skipROM() {
    if (!reset()) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }
    writeByte(0xCC);            // Skip ROM
    _err = AARUARDUINO_OK;
    return _err;
}

// Dallas/Maxim ROM search.
AaruarduinoError AaruOneWireEngine::searchROM(AaruOneWireROM* rom, AaruOneWireSearchState* state) {
    if (!rom || !state) {
        _err = AARUARDUINO_INVALID_CONFIG;
        return _err;
    }
    if (!reset()) {
        _err = AARUARDUINO_READ_FAIL;
        return _err;
    }
    writeByte(0xF0);            // Search ROM

    uint8_t id[8];
    uint8_t rom_byte_mask = 0;
    uint8_t rom_byte_no = 0;

    for (uint8_t bit = 0; bit < 64; bit++) {
        uint8_t id_bit = readBit();
        uint8_t cmp_bit = readBit();

        if (id_bit && cmp_bit) {
            // No device on the bus at this position.
            _err = AARUARDUINO_NOT_FOUND;
            return _err;
        }
        if (id_bit != cmp_bit) {
            // Differing bit means the branch is unique here.
            state->last_discrepancy = 0;
            state->last_family_discrepancy = 0;
            state->last_device_flag = true;
        } else {
            if (id_bit) {
                // Bit was 1 -> stay on the 1 branch.
            } else {
                // Bit was 0 -> stay on the 0 branch.
            }
            if (bit < state->last_discrepancy) {
                // keep the previous choice
            } else if (bit > state->last_discrepancy) {
                rom_byte_mask = 0;
                rom_byte_no = bit / 8;
            } else {
                // this bit is the discrepancy point
                rom_byte_mask = 1 << (bit % 8);
                if (state->last_device_flag) {
                    rom_byte_mask = 0;
                } else {
                    state->last_discrepancy = bit;
                }
            }
            if (rom_byte_mask) id[rom_byte_no] &= ~rom_byte_mask;
            else id[rom_byte_no] |= rom_byte_mask;
        }
    }

    // The first 56 bits are the ROM code, the last 8 are the CRC.
    rom->family = id[0];
    for (uint8_t i = 0; i < 6; i++) rom->serial[i] = id[1 + i];
    rom->crc = id[7];

    _err = checkCRC(rom) ? AARUARDUINO_OK : AARUARDUINO_CRC_ERROR;
    return _err;
}

int AaruOneWireEngine::discover(AaruOneWireROM* roms, int max_devices) {
    if (!roms || max_devices <= 0) {
        _err = AARUARDUINO_INVALID_CONFIG;
        return -1;
    }
    AaruOneWireSearchState state;
    state.last_discrepancy = 0;
    state.last_family_discrepancy = 0;
    state.last_device_flag = false;

    int found = 0;
    while (found < max_devices) {
        AaruOneWireROM rom;
        if (searchROM(&rom, &state) != AARUARDUINO_OK) break;
        roms[found++] = rom;
        if (state.last_discrepancy == 0 && state.last_device_flag) break;
    }
    _err = found > 0 ? AARUARDUINO_OK : AARUARDUINO_NOT_FOUND;
    return found > 0 ? found : -1;
}

// --- CRC -------------------------------------------------------------------

// Dallas/Maxim CRC-8, polynomial 0x31, initial value 0.
uint8_t AaruOneWireEngine::crc8(const uint8_t* data, uint8_t len) {
    if (!data) return 0;
    uint8_t crc = 0;
    for (uint8_t i = 0; i < len; i++) {
        uint8_t inbyte = data[i];
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}

bool AaruOneWireEngine::checkCRC(const AaruOneWireROM* rom) {
    if (!rom) return false;
    uint8_t buf[8];
    buf[0] = rom->family;
    for (uint8_t i = 0; i < 6; i++) buf[1 + i] = rom->serial[i];
    buf[7] = rom->crc;
    return crc8(buf, 7) == rom->crc;
}

// --- DS18B20 ---------------------------------------------------------------

AaruarduinoError AaruOneWireEngine::ds18b20Convert(const AaruOneWireROM* rom) {
    if (rom) {
        if (matchROM(rom) != AARUARDUINO_OK) return _err;
    } else {
        if (skipROM() != AARUARDUINO_OK) return _err;
    }
    writeByte(0x44);            // Convert Temperature
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruOneWireEngine::ds18b20ReadScratchpad(const AaruOneWireROM* rom, uint8_t* scratchpad) {
    if (!scratchpad) {
        _err = AARUARDUINO_INVALID_CONFIG;
        return _err;
    }
    if (rom) {
        if (matchROM(rom) != AARUARDUINO_OK) return _err;
    } else {
        if (skipROM() != AARUARDUINO_OK) return _err;
    }
    writeByte(0xBE);            // Read Scratchpad
    readBytes(scratchpad, 9);
    // Byte 8 is the CRC of bytes 0..7.
    if (crc8(scratchpad, 8) != scratchpad[8]) {
        _err = AARUARDUINO_CRC_ERROR;
        return _err;
    }
    _err = AARUARDUINO_OK;
    return _err;
}

float AaruOneWireEngine::ds18b20ReadTempC(const AaruOneWireROM* rom) {
    uint8_t sp[9];
    if (ds18b20ReadScratchpad(rom, sp) != AARUARDUINO_OK) return NAN;

    // Only the least significant 12 bits carry the temperature.
    int16_t raw = (int16_t)((uint16_t)sp[0] | ((uint16_t)sp[1] << 8));
    if (raw == 0x800) {
        // Power-on reset value: the conversion has not finished yet.
        _err = AARUARDUINO_NOT_INITIALIZED;
        return NAN;
    }
    _err = AARUARDUINO_OK;
    return (float)raw / 16.0f;
}

AaruarduinoError AaruOneWireEngine::ds18b20SetResolution(const AaruOneWireROM* rom, uint8_t resolution) {
    if (resolution < 9 || resolution > 12) {
        _err = AARUARDUINO_INVALID_CONFIG;
        return _err;
    }
    if (rom) {
        if (matchROM(rom) != AARUARDUINO_OK) return _err;
    } else {
        if (skipROM() != AARUARDUINO_OK) return _err;
    }
    writeByte(0x4E);            // Write Scratchpad
    writeByte(0);               // alarm 0
    writeByte(0);               // alarm 1
    writeByte((uint8_t)((resolution - 9) << 5));
    writeByte(0x7F);            // copy scratchpad
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruOneWireEngine::ds18b20SetAlarms(const AaruOneWireROM* rom, int8_t th, int8_t tl) {
    if (rom) {
        if (matchROM(rom) != AARUARDUINO_OK) return _err;
    } else {
        if (skipROM() != AARUARDUINO_OK) return _err;
    }
    writeByte(0x4E);            // Write Scratchpad
    writeByte((uint8_t)tl);
    writeByte((uint8_t)th);
    writeByte(0x7F);            // copy scratchpad
    _err = AARUARDUINO_OK;
    return _err;
}

void AaruOneWireEngine::delayUs(uint32_t us) {
#if defined(ARDUINO)
    delayMicroseconds(us);
#else
    (void)us;
#endif
}
