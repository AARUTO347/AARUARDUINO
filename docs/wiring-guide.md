# AARUARDUINO Wiring Guide
## MADE BY AARUTO

Text-only wiring diagrams and divider recommendations. Pin numbers assume Arduino Uno R3; adjust for your board using `docs/pin-reference.md`.

## Ultrasonic HC-SR04

```
Uno:  Trig -> D7, Echo -> D8 (via divider), VCC -> 5V, GND -> GND
ESP32: Trig -> GPIO5, Echo -> GPIO18 (via 1k/2k divider to 3.3V), VCC -> 5V, GND -> GND
```

Echo is 5 V. On 3.3 V boards use a divider: Echo -> 1k resistor -> GPIO, and 2k resistor from GPIO -> GND.

## DHT11 / DHT22

```
Data -> D2 (with 10k pullup to VCC), VCC -> 5V, GND -> GND
```

## I²C devices (SSD1306, LCD1602, BME280, DS3231, etc.)

```
Uno:   SDA -> A4, SCL -> A5, VCC -> 5V, GND -> GND
Mega:  SDA -> 20, SCL -> 21
ESP32: any GPIO (default 21/22)
ESP8266: any GPIO (default 4/5)
```

Add 4.7k pullups on SDA/SCL if the module lacks them.

## SPI devices (MAX7219, TFT, RFID, SD, flash)

```
Uno:   SS -> 10, MOSI -> 11, MISO -> 12, SCK -> 13
Mega:  SS -> 53, MOSI -> 51, MISO -> 50, SCK -> 52
ESP32: any GPIO (default SS=5, MOSI=23, MISO=19, SCK=18)
```

## 1-Wire (DS18B20)

```
Data -> D2 (with 4.7k pullup to VCC), VCC -> 5V, GND -> GND
```

## Analog sensors (LDR, LM35, MQ-x, soil, etc.)

```
Signal -> A0 (or any analog pin), VCC -> 5V/3.3V, GND -> GND
MQ sensors need a heater supply (5V) and a load resistor (typically 10k).
```

## PWM outputs (LED, buzzer, motor enable, servo)

```
PWM pin -> module signal, GND -> common GND
Servo signal -> PWM pin, servo VCC/GND -> external 5V supply (not GPIO!)
```

## Relays / solenoids / lasers

```
Relay IN -> digital pin, relay VCC/GND -> external supply, common GND with board
Never drive a relay coil directly from GPIO — use a transistor/module.
```

## Motors (DC via H-bridge)

```
IN1/IN2 -> digital pins, EN -> PWM pin, motor power -> external supply, common GND
```

## Safety reminders

- ESP32/ESP8266 GPIO is 3.3 V only — never connect 5 V signals directly.
- Use external power for motors, relays, servos, heaters, pumps.
- Common GND between board and all external power supplies.
- Fuse high-current loads.
