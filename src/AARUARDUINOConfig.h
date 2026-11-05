// ============================================================
//  AARUARDUINO Configuration
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_CONFIG_H
#define AARUARDUINO_CONFIG_H

// ---- Feature opt-out macros (define before including AARUARDUINO.h to save flash/RAM) ----
// #define AARUARDUINO_NO_ANALOG
// #define AARUARDUINO_NO_DIGITAL
// #define AARUARDUINO_NO_I2C
// #define AARUARDUINO_NO_SPI
// #define AARUARDUINO_NO_UART
// #define AARUARDUINO_NO_ONEWIRE
// #define AARUARDUINO_NO_PWM
// #define AARUARDUINO_NO_DISPLAYS
// #define AARUARDUINO_NO_ACTUATORS
// #define AARUARDUINO_NO_RADIO
// #define AARUARDUINO_NO_STORAGE
// #define AARUARDUINO_NO_SPECIAL

// ---- Timing defaults ----
#define AARUARDUINO_DEFAULT_ULTRASONIC_TIMEOUT_US   30000UL
#define AARUARDUINO_DEFAULT_I2C_TIMEOUT_MS          100
#define AARUARDUINO_DEFAULT_SPI_SPEED_HZ            4000000UL
#define AARUARDUINO_DEFAULT_UART_TIMEOUT_MS         1000
#define AARUARDUINO_DEFAULT_ONEWIRE_TIMEOUT_US      250000UL
#define AARUARDUINO_DEFAULT_PULSE_TIMEOUT_US        1000000UL

// ---- Debounce ----
#define AARUARDUINO_DEFAULT_DEBOUNCE_MS             50

// ---- Analog engine ----
#define AARUARDUINO_ANALOG_SAMPLES                  8
#define AARUARDUINO_ANALOG_SETTLE_US                10

// ---- Display ----
#define AARUARDUINO_DISPLAY_DEFAULT_I2C_ADDR        0x3C
#define AARUARDUINO_LCD_DEFAULT_I2C_ADDR            0x27

// ---- DHT ----
#define AARUARDUINO_DHT_MIN_INTERVAL_MS             2000

// ---- Debug ----
// #define AARUARDUINO_DEBUG 1

#ifndef AARUARDUINO_ASSERT
  #define AARUARDUINO_ASSERT(cond) ((void)0)
#endif

#endif // AARUARDUINO_CONFIG_H
