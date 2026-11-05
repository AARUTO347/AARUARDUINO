// ============================================================
//  AARUARDUINO v1.0.0 — Universal Arduino Library
//  MADE BY AARUTO
//  1000+ modules, one API.
// ============================================================
#ifndef AARUARDUINO_TYPES_H
#define AARUARDUINO_TYPES_H

#include <stdint.h>

// ---- Version ----
#define AARUARDUINO_VERSION_MAJOR 1
#define AARUARDUINO_VERSION_MINOR 0
#define AARUARDUINO_VERSION_PATCH 0
#define AARUARDUINO_VERSION_STRING "1.0.0"

// ---- Board family enum ----
typedef enum {
    AARUARDUINO_FAMILY_UNKNOWN       = 0,
    AARUARDUINO_FAMILY_UNO_R3        = 1,
    AARUARDUINO_FAMILY_UNO_R4_MINIMA = 2,
    AARUARDUINO_FAMILY_UNO_R4_WIFI   = 3,
    AARUARDUINO_FAMILY_MEGA          = 4,
    AARUARDUINO_FAMILY_LEONARDO      = 5,
    AARUARDUINO_FAMILY_MICRO         = 6,
    AARUARDUINO_FAMILY_DUE           = 7,
    AARUARDUINO_FAMILY_ZERO          = 8,
    AARUARDUINO_FAMILY_NANO_33       = 9,
    AARUARDUINO_FAMILY_ESP32         = 10,
    AARUARDUINO_FAMILY_ESP8266       = 11
} AaruarduinoFamily;

// ---- Error codes ----
typedef enum {
    AARUARDUINO_OK               = 0,
    AARUARDUINO_ERROR            = 1,
    AARUARDUINO_INVALID_PIN      = 2,
    AARUARDUINO_INVALID_CONFIG   = 3,
    AARUARDUINO_TIMEOUT          = 4,
    AARUARDUINO_READ_FAIL        = 5,
    AARUARDUINO_WRITE_FAIL       = 6,
    AARUARDUINO_UNSUPPORTED      = 7,
    AARUARDUINO_MISSING_DEP      = 8,
    AARUARDUINO_INIT_FAIL        = 9,
    AARUARDUINO_NOT_INITIALIZED  = 10,
    AARUARDUINO_BUSY             = 11,
    AARUARDUINO_BUFFER_OVERFLOW  = 12,
    AARUARDUINO_CRC_ERROR        = 13,
    AARUARDUINO_NOT_FOUND        = 14
} AaruarduinoError;

// ---- Module categories ----
typedef enum {
    AARUARDUINO_CAT_ANALOG      = 1,
    AARUARDUINO_CAT_DIGITAL     = 2,
    AARUARDUINO_CAT_I2C         = 3,
    AARUARDUINO_CAT_SPI         = 4,
    AARUARDUINO_CAT_UART        = 5,
    AARUARDUINO_CAT_ONEWIRE     = 6,
    AARUARDUINO_CAT_PWM         = 7,
    AARUARDUINO_CAT_DISPLAY     = 8,
    AARUARDUINO_CAT_ACTUATOR    = 9,
    AARUARDUINO_CAT_RADIO       = 10,
    AARUARDUINO_CAT_STORAGE     = 11,
    AARUARDUINO_CAT_SPECIAL     = 12
} AaruarduinoCategory;

// ---- Module status ----
typedef enum {
    AARUARDUINO_MOD_IMPLEMENTED   = 1,
    AARUARDUINO_MOD_STUB          = 2,
    AARUARDUINO_MOD_PLANNED       = 3
} AaruarduinoModuleStatus;

// ---- Forward decl for singleton ----
class AaruarduinoClass;

#endif // AARUARDUINO_TYPES_H
