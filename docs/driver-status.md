# AARUARDUINO Driver Status
## MADE BY AARUTO

This page lists how complete each hardware driver is, so you know exactly what
you get before wiring something up.

AARUARDUINO never returns invented sensor values. When a driver cannot talk to
its hardware it returns `NaN` (float), `-1` (int) or `false` (bool) and sets an
error code that you can read with `AARUARDUINO.lastError()`.

### Fully implemented (real protocol, verified by host tests)

| Driver | Bus | Notes |
|---|---|---|
| HC-SR04 ultrasonic | GPIO | Trigger pulse, echo timeout, cm and inches |
| DHT11 / DHT22 | GPIO | Own bit-bang protocol, checksum check, 12/22 decoding |
| BME280 / BMP280 | I2C | Chip ID, calibration, Bosch compensation, control registers |
| SSD1306 OLED | I2C / SPI | Framebuffer, 5x7 font, text and graphics primitives |
| LCD1602 (PCF8574) | I2C | 4-bit mode, custom chars, scroll, backlight |
| MAX7219 | SPI | Chained devices, 16 registers, digits, matrix, brightness |
| BH1750 | I2C | Lux measurement, MTreg scaling, power modes |
| TM1637 | GPIO | 2-wire protocol, digits, clock, colon |
| DS3231 RTC | I2C | Time, alarms, square wave, temperature, SRAM |
| MPU6050 / MPU9250 | I2C | WHO_AM_I, accel, gyro, temperature, calibration |
| DS18B20 | 1-Wire | ROM search, CRC-8, scratchpad, conversion, alarms |
| GPS / NEO-6M family | UART | NMEA GGA / RMC / GSV / VTG with checksum validation |
| IR remote | GPIO | NEC and Sony receive (pin-change capture) and send |
| Stepper (A4988, DRV8825, TB6600) | GPIO | Speed ramping, position moves, enable |
| Servo | GPIO | Angle and microsecond positioning |
| DC motor, relay, solenoid, LED, buzzer, RGB | GPIO | PWM and digital outputs |

### How to check at runtime

```cpp
if (AARUARDUINO.lastError() != AARUARDUINO_OK) {
  Serial.println(AARUARDUINO.errorString());
}
```

### Partially implemented

| Driver | What works | What is missing |
|---|---|---|
| RC5 / RC6 IR | - | Manchester encoding is not implemented; the driver reports `AARUARDUINO_UNSUPPORTED` |
| WS2812 on AVR / SAMD | Pixel buffer, colours, effects | The 800 kHz bit-bang cannot be timed reliably with `digitalWrite()`, so `show()` reports `AARUARDUINO_UNSUPPORTED` there. ESP32 works. |
| BME280 over SPI | Setup | Only the I2C transfer path is implemented |

Error codes: `OK`, `ERROR`, `INVALID_PIN`, `INVALID_CONFIG`, `TIMEOUT`,
`READ_FAIL`, `WRITE_FAIL`, `UNSUPPORTED`, `MISSING_DEP`, `INIT_FAIL`,
`NOT_INITIALIZED`, `BUSY`, `BUFFER_OVERFLOW`, `CRC_ERROR`, `NOT_FOUND`.
