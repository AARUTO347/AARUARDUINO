# Changelog

## v1.0.4
### New
- Real NMEA 0183 parser for the GPS driver (GGA, RMC, GSV, VTG) with checksum validation
- `AaruGPS::feedSentence()` for feeding NMEA from any source
- Shared bus setup API: `i2cBegin()`, `spiBegin()`, `uartBegin()`, `i2cDevicePresent()`, `i2cScanFirst()`
- Native C++ host test suite (`tests/host/native/`, 92 assertions): GPS/NMEA, SSD1306,
  1-Wire CRC, BME280
- Static analysis tools: `tools/check_definitions.py`, `tools/check_init.py`

### Drivers now fully implemented
- **GPS / NMEA** - GGA, RMC, GSV and VTG sentences, `*HH` checksum validation,
  hemisphere handling, satellite table
- **1-Wire / DS18B20** - reset pulse, read/write ROM, ROM search, Dallas CRC-8,
  scratchpad read with CRC check, temperature conversion, resolution and alarm setup
- **BME280 / BMP280** - chip detection, calibration block read, Bosch temperature,
  pressure and humidity compensation, oversampling / filter / standby / mode control
- **SSD1306** - framebuffer, init sequence, page addressing, 5x7 font, text, lines,
  rectangles, circles and bitmaps over I2C and SPI
- **TM1637** - two-wire protocol (START / data / ACK / STOP), segment encoding,
  numbers, floats and clock display
- **MAX7219** - no-op cascade across chained devices, all 16 registers, digit and
  matrix modes, number and float display, intensity, scan limit, shutdown
- **LCD1602 (PCF8574 backpack)** - 4-bit initialisation, commands, DDRAM/CGRAM
  addressing, custom characters, auto-scroll, backlight
- **BH1750** - command set, continuous and one-shot modes, MTreg scaling, lux
  calculation
- **DS3231** - date/time read and write, BCD conversion, unix time conversion,
  temperature, alarms 1 and 2, square wave output, aging offset, SRAM
- **IR remote** - NEC and Sony receive (pin-change capture) and send (bit banged),
  raw burst send. RC5/RC6 report `AARUARDUINO_UNSUPPORTED` instead of pretending
- **WS2812 / NeoPixel** - GRB packing, HSV conversion, rainbow, theatre chase,
  colour wipe (the 800 kHz bit-bang is only available where it can be timed
  accurately; other cores report `AARUARDUINO_UNSUPPORTED` instead of lying)

### Fixed
- 43 declared methods had no definition (would fail to link when called)
- 158 class members started with garbage state - every driver and engine now
  initialises its members, so `hasFix()`/`isConfigured()`/`lastError()` are
  meaningful before `begin()`
- SPI bus was never initialised, so MAX7219 could not talk to hardware
- RMC sentences never marked the fix as valid
- `AaruUARTEngine` had no `setTimeout()`/`isBegun()`/`lastError()` implementations
- `AaruUARTEngine::nmeaValidate()` always returned true
- SoftwareSerial is now detected with `__has_include`, so cores without it
  (e.g. some RENESAS_UNO builds) still compile and fall back to a hardware UART
- ESP32/ESP8266 UART setup now uses the correct board-specific serial config
- ESP32 `SPI.begin()` is called with real pins only when the user supplied them

### Verified
- 20/20 examples compile on Arduino Uno R3, Mega 2560, ESP32 and ESP8266
- 92/92 native host assertions pass
- 36/36 pure-math host tests pass
- Font moved to PROGMEM on AVR - the library fits in the Uno's 2 kB of RAM again


## v1.0.3
- Reduced static RAM usage (~170 bytes) by shrinking GPS satellite table and NMEA buffer
- Fixes low-memory warning on Arduino Uno R3 (now 73% RAM instead of 82%)


## v1.0.2
- Fixed AaruGPS accessors (hasFix/lat/lon/alt/speed/course/sats/date/time) missing from build
- Fixed displayPrint() overload ambiguity for long / unsigned long arguments
- Added display routing so display* helpers work with OLED, LCD1602, MAX7219 and TM1637
- Added WS2812 convenience API (ws2812Set/Show/Fill/Clear/Rainbow/Brightness)
- Restored default arguments across all driver headers
- Fixed AaruStepper duplicate moveTo() overload
- Fixed library.properties url field (Arduino Library Manager lint LP042)


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
