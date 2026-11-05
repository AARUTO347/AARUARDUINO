// ============================================================
//  AARUARDUINO Board Detection & Pin Validation
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_BOARD_H
#define AARUARDUINO_BOARD_H

#include "AARUARDUINOTypes.h"
#include "utility/AaruarduinoMath.h"

// ---- Compile-time board detection (never guess from GPIO numbers) ----
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
  #define AARUARDUINO_BOARD_ESP32 1
  #define AARUARDUINO_BOARD_NAME "ESP32"
  #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_ESP32
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
  #define AARUARDUINO_BOARD_ESP8266 1
  #define AARUARDUINO_BOARD_NAME "ESP8266"
  #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_ESP8266
#elif defined(ARDUINO_UNOR4_WIFI)
  #define AARUARDUINO_BOARD_UNO_R4_WIFI 1
  #define AARUARDUINO_BOARD_NAME "Uno R4 WiFi"
  #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_UNO_R4_WIFI
#elif defined(ARDUINO_UNOR4_MINIMA) || defined(ARDUINO_MINIMA)
  #define AARUARDUINO_BOARD_UNO_R4_MINIMA 1
  #define AARUARDUINO_BOARD_NAME "Uno R4 Minima"
  #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_UNO_R4_MINIMA
#elif defined(__AVR_ATmega2560__) || defined(ARDUINO_AVR_MEGA2560)
  #define AARUARDUINO_BOARD_MEGA 1
  #define AARUARDUINO_BOARD_NAME "Mega 2560"
  #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_MEGA
#elif defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO)
  #if defined(ARDUINO_AVR_MICRO)
    #define AARUARDUINO_BOARD_MICRO 1
    #define AARUARDUINO_BOARD_NAME "Micro"
    #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_MICRO
  #else
    #define AARUARDUINO_BOARD_LEONARDO 1
    #define AARUARDUINO_BOARD_NAME "Leonardo"
    #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_LEONARDO
  #endif
#elif defined(__SAM3X8E__) || defined(ARDUINO_SAM_DUE)
  #define AARUARDUINO_BOARD_DUE 1
  #define AARUARDUINO_BOARD_NAME "Due"
  #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_DUE
#elif defined(__SAMD21G18A__) || defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_NANO_33_IOT)
  #if defined(ARDUINO_SAMD_NANO_33_IOT)
    #define AARUARDUINO_BOARD_NANO_33 1
    #define AARUARDUINO_BOARD_NAME "Nano 33 IoT"
    #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_NANO_33
  #else
    #define AARUARDUINO_BOARD_ZERO 1
    #define AARUARDUINO_BOARD_NAME "Zero"
    #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_ZERO
  #endif
#elif defined(__AVR_ATmega328P__) || defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)
  #define AARUARDUINO_BOARD_UNO_R3 1
  #define AARUARDUINO_BOARD_NAME "Uno R3"
  #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_UNO_R3
#else
  #define AARUARDUINO_BOARD_UNKNOWN 1
  #define AARUARDUINO_BOARD_NAME "Unknown"
  #define AARUARDUINO_BOARD_FAMILY AARUARDUINO_FAMILY_UNKNOWN
#endif

// ADC characteristics per family.
#if defined(AARUARDUINO_BOARD_ESP32)
  #define AARUARDUINO_ADC_BITS 12
  #define AARUARDUINO_ADC_MAX 4095
  #define AARUARDUINO_ADC_VREF 3.3f
#elif defined(AARUARDUINO_BOARD_ESP8266)
  #define AARUARDUINO_ADC_BITS 10
  #define AARUARDUINO_ADC_MAX 1023
  #define AARUARDUINO_ADC_VREF 3.3f
#elif defined(AARUARDUINO_BOARD_UNO_R4_MINIMA) || defined(AARUARDUINO_BOARD_UNO_R4_WIFI)
  #define AARUARDUINO_ADC_BITS 14
  #define AARUARDUINO_ADC_MAX 16383
  #define AARUARDUINO_ADC_VREF 5.0f
#elif defined(AARUARDUINO_BOARD_DUE) || defined(AARUARDUINO_BOARD_ZERO) || defined(AARUARDUINO_BOARD_NANO_33)
  #define AARUARDUINO_ADC_BITS 12
  #define AARUARDUINO_ADC_MAX 4095
  #define AARUARDUINO_ADC_VREF 3.3f
#else
  // Uno R3, Mega, Leonardo, Micro, Unknown fallback: 10-bit AVR ADC.
  #define AARUARDUINO_ADC_BITS 10
  #define AARUARDUINO_ADC_MAX 1023
  #define AARUARDUINO_ADC_VREF 5.0f
#endif

// Returns true if a pin can only be an input (e.g. ESP32 34-39).
inline bool aaruarduinoIsInputOnlyPin(uint8_t pin) {
#if defined(AARUARDUINO_BOARD_ESP32)
  return (pin >= 34 && pin <= 39);
#else
  (void)pin;
  return false;
#endif
}

// Returns true for ESP strapping / boot-sensitive pins (usable with care).
inline bool aaruarduinoIsStrappingPin(uint8_t pin) {
#if defined(AARUARDUINO_BOARD_ESP32)
  return (pin == 0 || pin == 2 || pin == 5 || pin == 12 || pin == 15);
#elif defined(AARUARDUINO_BOARD_ESP8266)
  return (pin == 0 || pin == 2 || pin == 15 || pin == 16);
#else
  (void)pin;
  return false;
#endif
}

// Validate a digital GPIO number where detectable. Returns AaruarduinoError.
// NOTE (unknown boards): validation is permissive by design so new variants
// still compile; the Arduino core enforces the final limits. See docs.
inline AaruarduinoError aaruarduinoValidateDigitalPin(uint8_t pin, bool needOutput) {
#if defined(AARUARDUINO_BOARD_UNO_R3) || defined(AARUARDUINO_BOARD_UNO_R4_MINIMA) || defined(AARUARDUINO_BOARD_UNO_R4_WIFI)
  // Uno: digital 0-13, analog A0-A5 also usable as digital.
  // A0..A5 map to 14..19 on AVR, 14..19+ on Renesas; accept 0..19.
  if (pin > 19) return AARUARDUINO_INVALID_PIN;
  (void)needOutput;
  return AARUARDUINO_OK;
#elif defined(AARUARDUINO_BOARD_MEGA)
  // Mega: digital 0-53, analog A0-A15 also usable as digital.
  // A0..A15 map to 54..69; accept 0..69.
  if (pin > 69) return AARUARDUINO_INVALID_PIN;
  (void)needOutput;
  return AARUARDUINO_OK;
#elif defined(AARUARDUINO_BOARD_LEONARDO) || defined(AARUARDUINO_BOARD_MICRO)
  // Leonardo/Micro: digital 0-23, analog A0-A11 also usable as digital.
  // A0..A11 map to 18..29; accept 0..29.
  if (pin > 29) return AARUARDUINO_INVALID_PIN;
  (void)needOutput;
  return AARUARDUINO_OK;
#elif defined(AARUARDUINO_BOARD_DUE)
  // Due: digital 0-53, analog A0-A11 also usable as digital.
  // A0..A11 map to 54..65; accept 0..65.
  if (pin > 65) return AARUARDUINO_INVALID_PIN;
  (void)needOutput;
  return AARUARDUINO_OK;
#elif defined(AARUARDUINO_BOARD_ZERO) || defined(AARUARDUINO_BOARD_NANO_33)
  // Zero/Nano 33: digital 0-21, analog A0-A5 also usable as digital.
  // A0..A5 map to 14..19; accept 0..21.
  if (pin > 21) return AARUARDUINO_INVALID_PIN;
  (void)needOutput;
  return AARUARDUINO_OK;
#elif defined(AARUARDUINO_BOARD_ESP8266)
  // ESP8266: common usable GPIOs.
  switch (pin) {
    case 0: case 1: case 2: case 3: case 4: case 5:
    case 12: case 13: case 14: case 15: case 16:
      return AARUARDUINO_OK;
    default:
      return AARUARDUINO_INVALID_PIN;
  }
#elif defined(AARUARDUINO_BOARD_ESP32)
  // ESP32: reject flash pins + out-of-range; flag input-only when output needed.
  if (pin > 39) return AARUARDUINO_INVALID_PIN;
  if (pin >= 6 && pin <= 11) return AARUARDUINO_INVALID_PIN;  // SPI flash
  if (needOutput && aaruarduinoIsInputOnlyPin(pin)) return AARUARDUINO_INVALID_PIN;
  return AARUARDUINO_OK;
#else
  // Unknown board: permissive fallback (documented)
  (void)pin; (void)needOutput;
  return AARUARDUINO_OK;
#endif
}

// Validate that a pin supports PWM output on this board.
inline AaruarduinoError aaruarduinoValidatePwmPin(uint8_t pin) {
  AaruarduinoError d = aaruarduinoValidateDigitalPin(pin, true);
  if (d != AARUARDUINO_OK) return d;
#if defined(AARUARDUINO_BOARD_UNO_R3) || defined(AARUARDUINO_BOARD_UNO_R4_MINIMA) || defined(AARUARDUINO_BOARD_UNO_R4_WIFI)
  // Uno R3 + R4: hardware PWM on 3,5,6,9,10,11.
  switch (pin) {
    case 3: case 5: case 6: case 9: case 10: case 11:
      return AARUARDUINO_OK;
    default:
      return AARUARDUINO_INVALID_PIN;
  }
#elif defined(AARUARDUINO_BOARD_MEGA)
  // Mega: PWM on 2-13, 44-46.
  if ((pin >= 2 && pin <= 13) || (pin >= 44 && pin <= 46)) return AARUARDUINO_OK;
  return AARUARDUINO_INVALID_PIN;
#elif defined(AARUARDUINO_BOARD_LEONARDO) || defined(AARUARDUINO_BOARD_MICRO)
  // Leonardo/Micro: PWM on 3,5,6,9,10,11,13.
  switch (pin) {
    case 3: case 5: case 6: case 9: case 10: case 11: case 13:
      return AARUARDUINO_OK;
    default:
      return AARUARDUINO_INVALID_PIN;
  }
#elif defined(AARUARDUINO_BOARD_DUE)
  // Due: PWM on 2-13 + DAC0/DAC1.
  if (pin >= 2 && pin <= 13) return AARUARDUINO_OK;
  return AARUARDUINO_INVALID_PIN;
#elif defined(AARUARDUINO_BOARD_ZERO) || defined(AARUARDUINO_BOARD_NANO_33)
  // SAMD21: PWM on most pins via TC/TCC.
  if (pin <= 21) return AARUARDUINO_OK;
  return AARUARDUINO_INVALID_PIN;
#elif defined(AARUARDUINO_BOARD_ESP8266)
  // ESP8266: software PWM on most GPIOs; 16 has no PWM/interrupts.
  if (pin == 16) return AARUARDUINO_INVALID_PIN;
  return AARUARDUINO_OK;
#elif defined(AARUARDUINO_BOARD_ESP32)
  // ESP32: LEDC on any output GPIO.
  return AARUARDUINO_OK;
#else
  return AARUARDUINO_OK;
#endif
}

// Validate that a pin is a usable analog input. Accepts Arduino pin numbers
// (including A0..Ax aliases). Returns AaruarduinoError.
inline AaruarduinoError aaruarduinoValidateAnalogPin(int pin) {
  if (pin < 0) return AARUARDUINO_INVALID_PIN;
#if defined(AARUARDUINO_BOARD_ESP8266)
  // ESP8266 has a single ADC: A0 only. In the Arduino core A0 == 17.
#if defined(A0)
  if (pin == (int)A0 || pin == 17) return AARUARDUINO_OK;
#else
  if (pin == 17 || pin == 0) return AARUARDUINO_OK;
#endif
  return AARUARDUINO_INVALID_PIN;
#elif defined(AARUARDUINO_BOARD_ESP32)
  // ESP32: ADC1 (32-39, safe with WiFi) + ADC2 (0,2,4,12-15,25-27, WiFi conflicts).
  switch (pin) {
    case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 39:
    case 0: case 2: case 4: case 12: case 13: case 14: case 15:
    case 25: case 26: case 27:
      return AARUARDUINO_OK;
    default:
      return AARUARDUINO_INVALID_PIN;
  }
#elif defined(AARUARDUINO_BOARD_UNO_R3) || defined(AARUARDUINO_BOARD_UNO_R4_MINIMA) || defined(AARUARDUINO_BOARD_UNO_R4_WIFI)
  // Accept A0..A5 aliases (14..19) and channel numbers 0..5.
#if defined(A0) && defined(A5)
  if (pin >= (int)A0 && pin <= (int)A5) return AARUARDUINO_OK;
#endif
  if ((pin >= 14 && pin <= 19) || (pin >= 0 && pin <= 5)) return AARUARDUINO_OK;
  return AARUARDUINO_INVALID_PIN;
#elif defined(AARUARDUINO_BOARD_MEGA)
  // Accept A0..A15 aliases (54..69) and channel numbers 0..15.
#if defined(A0) && defined(A15)
  if (pin >= (int)A0 && pin <= (int)A15) return AARUARDUINO_OK;
#endif
  if ((pin >= 54 && pin <= 69) || (pin >= 0 && pin <= 15)) return AARUARDUINO_OK;
  return AARUARDUINO_INVALID_PIN;
#elif defined(AARUARDUINO_BOARD_LEONARDO) || defined(AARUARDUINO_BOARD_MICRO)
  // Accept A0..A11 aliases (18..29) and channel numbers 0..11.
#if defined(A0) && defined(A11)
  if (pin >= (int)A0 && pin <= (int)A11) return AARUARDUINO_OK;
#endif
  if ((pin >= 18 && pin <= 29) || (pin >= 0 && pin <= 11)) return AARUARDUINO_OK;
  return AARUARDUINO_INVALID_PIN;
#elif defined(AARUARDUINO_BOARD_DUE)
  // Accept A0..A11 aliases (54..65) and channel numbers 0..11.
#if defined(A0) && defined(A11)
  if (pin >= (int)A0 && pin <= (int)A11) return AARUARDUINO_OK;
#endif
  if ((pin >= 54 && pin <= 65) || (pin >= 0 && pin <= 11)) return AARUARDUINO_OK;
  return AARUARDUINO_INVALID_PIN;
#elif defined(AARUARDUINO_BOARD_ZERO) || defined(AARUARDUINO_BOARD_NANO_33)
  // Accept A0..A5 aliases (14..19) and channel numbers 0..5.
#if defined(A0) && defined(A5)
  if (pin >= (int)A0 && pin <= (int)A5) return AARUARDUINO_OK;
#endif
  if ((pin >= 14 && pin <= 19) || (pin >= 0 && pin <= 5)) return AARUARDUINO_OK;
  return AARUARDUINO_INVALID_PIN;
#else
  (void)pin;
  return AARUARDUINO_OK;
#endif
}

// ---- Board family getters (for runtime checks) ----
inline AaruarduinoFamily aaruarduinoGetBoardFamily() {
  return (AaruarduinoFamily)AARUARDUINO_BOARD_FAMILY;
}

inline const char* aaruarduinoGetBoardName() {
  return AARUARDUINO_BOARD_NAME;
}

#endif // AARUARDUINO_BOARD_H