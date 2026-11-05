# AARUARDUINO Board Compatibility
## MADE BY AARUTO

| Board | Family | ADC | PWM pins | I²C | SPI | UART | Notes |
|---|---|---|---|---|---|---|---|
| Uno R3 | AVR ATmega328P | 10-bit, 5 V | 3,5,6,9,10,11 | A4/A5 | 10-13 | 1 (TX/RX) | 5 V logic |
| Uno R4 Minima | Renesas RA4M1 | 14-bit, 5 V | 3,5,6,9,10,11 | A4/A5 | 10-13 | 1 | 5 V logic |
| Uno R4 WiFi | Renesas RA4M1 + ESP32-S3 | 14-bit, 5 V | 3,5,6,9,10,11 | A4/A5 | 10-13 | 1 | WiFi via ESP32-S3 |
| Nano | AVR ATmega328P | 10-bit, 5 V | 3,5,6,9,10,11 | A4/A5 | 10-13 | 1 | 5 V logic |
| Mega 2560 | AVR ATmega2560 | 10-bit, 5 V | 2-13,44-46 | 20/21 | 50-53 | 1-4 | 5 V logic, many UARTs |
| Leonardo / Micro | AVR ATmega32U4 | 10-bit, 5 V | 3,5,6,9,10,11,13 | 2/3 | ICSP | 1 | Native USB |
| Due | SAM3X8E Cortex-M3 | 12-bit, 3.3 V | 2-13 | 20/21 | SPI header | 1-4 | 3.3 V logic |
| Zero | SAMD21 Cortex-M0+ | 12-bit, 3.3 V | most | SDA/SCL | SPI header | 1 | 3.3 V logic |
| Nano 33 IoT | SAMD21 + nRF52 | 12-bit, 3.3 V | most | A4/A5 | SPI header | 1 | 3.3 V logic, BLE |
| ESP32 | ESP32 family | 12-bit, 3.3 V | any output | any GPIO | any GPIO | 1-3 | 3.3 V, NOT 5 V tolerant |
| ESP8266 | ESP8266 family | 10-bit, 3.3 V | most except 16 | any GPIO | any GPIO | 1 | 3.3 V, single ADC (A0) |

## Voltage safety

- 5 V boards (Uno, Nano, Mega, Leonardo, Micro, Due, Zero): 5 V tolerant (Due/Zero are 3.3 V — check datasheet).
- ESP32/ESP8266: **3.3 V only, not 5 V tolerant**. HC-SR04 echo (5 V) needs a voltage divider (1k/2k) or level shifter. Software cannot fix wiring.
- Never drive motors, relays, servos or high-current loads directly from GPIO. Use driver modules + external power.

## Pin validation

`AARUARDUINO.isValidDigitalPin(pin)`, `isValidAnalogPin(pin)`, `isValidPwmPin(pin)` perform compile-time-aware runtime validation per board. Invalid pins return `AARUARDUINO_INVALID_PIN`.
