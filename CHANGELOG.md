# Changelog

## v1.0.0 — 2026-11-05
- Initial release of AARUARDUINO by AARUTO
- 1136 modules across analog, digital, I²C, SPI, UART, 1-Wire, PWM, display, actuator, radio, storage, special
- Singleton API: `AARUARDUINO.begin()/update()/readX()`
- 8 shared engines: Analog, Digital, PWM, I²C, SPI, UART, 1-Wire, Pulse
- 15 drivers: HC-SR04, DHT11/22, MPU6050, DS18B20, DS3231, BME280/BMP280, BH1750, SSD1306, LCD1602, MAX7219, TM1637, WS2812, GPS/NMEA, IR Remote (NEC/Sony/RC5/RC6), Stepper
- Board support: Uno R3/R4, Nano, Mega, Leonardo, Micro, Due, Zero, Nano 33 IoT, ESP32, ESP8266
- No mandatory third-party dependencies; optional deps guarded
- Host tests (Python), registry validator, keywords/docs generators, structure validator
- MIT license
