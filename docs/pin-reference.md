# AARUARDUINO Pin Reference
## MADE BY AARUTO

Safe pins per board for digital, analog and PWM operations. Use `isValidDigitalPin()`, `isValidAnalogPin()`, `isValidPwmPin()` to validate at runtime.

## Arduino Uno R3 / Nano

- Digital: 0-13 (A0-A5 also usable as digital, mapped to 14-19)
- Analog input: A0-A5 (or 14-19, or channels 0-5)
- PWM: 3, 5, 6, 9, 10, 11
- I²C: SDA=A4, SCL=A5
- SPI: SS=10, MOSI=11, MISO=12, SCK=13
- UART: TX=1, RX=0 (shared with USB serial)

## Arduino Uno R4 Minima / WiFi

- Digital: 0-13, A0-A5 usable
- Analog input: A0-A5 (14-bit ADC)
- PWM: 3, 5, 6, 9, 10, 11
- I²C: SDA=A4, SCL=A5
- SPI: 10-13
- UART: 1 (TX/RX)

## Arduino Mega 2560

- Digital: 0-53, A0-A15 usable (54-69)
- Analog input: A0-A15 (channels 0-15)
- PWM: 2-13, 44-46
- I²C: SDA=20, SCL=21
- SPI: SS=53, MOSI=51, MISO=50, SCK=52
- UART: 1-4

## Arduino Leonardo / Micro

- Digital: 0-23, A0-A11 usable (18-29)
- Analog input: A0-A11 (channels 0-11)
- PWM: 3, 5, 6, 9, 10, 11, 13
- I²C: SDA=2, SCL=3
- SPI: ICSP header
- UART: 1 (TX/RX, native USB on 0/1)

## Arduino Due

- Digital: 0-53, A0-A11 usable (54-65)
- Analog input: A0-A11 (12-bit, channels 0-11)
- PWM: 2-13
- I²C: SDA=20, SCL=21
- SPI: ICSP header
- UART: 1-4

## Arduino Zero / Nano 33 IoT

- Digital: 0-19, A0-A5 usable (14-19)
- Analog input: A0-A5 (12-bit, channels 0-5)
- PWM: most pins 0-21
- I²C: SDA=A4, SCL=A5 (Zero); SDA/SCL pins (Nano 33)
- SPI: ICSP header
- UART: 1

## ESP32

- Digital: 0-39 (reject 6-11 = flash, 34-39 = input-only)
- Analog input: 32-39 (ADC1, safe with WiFi), 0,2,4,12-15,25-27 (ADC2, WiFi conflicts)
- PWM: any output GPIO via LEDC
- I²C: any GPIO (default 21/22)
- SPI: any GPIO (default SS=5, MOSI=23, MISO=19, SCK=18)
- UART: 1-3
- Strapping pins (use with care): 0, 2, 5, 12, 15
- Input-only: 34-39

## ESP8266 (NodeMCU / Wemos D1 mini)

- Usable digital: 0,1,2,3,4,5,12,13,14,15,16 (16 no PWM/interrupt)
- Analog input: A0 only (channel 0, 10-bit)
- PWM: most GPIOs except 16
- I²C: any GPIO
- SPI: any GPIO
- UART: 1 (TX/RX)
- Strapping pins: 0, 2, 15, 16
