<div align="center">

<img src="assets/logo.png" alt="AARUARDUINO Logo" width="200"/>

# AARUARDUINO

### Universal Arduino Library — 1000+ Modules, One Consistent API

[![Version](https://img.shields.io/badge/version-1.0.3-00979D?style=for-the-badge&logo=arduino)]()
[![Modules](https://img.shields.io/badge/modules-1136-green?style=for-the-badge)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](LICENSE)
[![Arduino](https://img.shields.io/badge/Arduino-Uno%20%7C%20Mega%20%7C%20ESP32%20%7C%20ESP8266-00979D?style=for-the-badge&logo=arduino)]()

**Made by [AARUTO](https://github.com/AARUTO347) — MADE BY AARUTO**

[Quick Start](#-quick-start) • [Installation](#-installation) • [Modules](#-module-reference) • [API](#-api) • [Documentation](#-documentation)

</div>

---

## About

AARUARDUINO provides **one consistent API** for 1000+ sensors, actuators, displays, and communication modules across all Arduino IDE-compatible boards. No more juggling different libraries — just `#include <AARUARDUINO.h>` and go.

## Supported Boards

| Board | Architecture | Voltage |
|-------|:-----------:|:-------:|
| Arduino Uno R3 | ATmega328P | 5V |
| Arduino Uno R4 Minima | RA4M1 | 5V |
| Arduino Uno R4 WiFi | RA4M1 + ESP32-S3 | 5V |
| Arduino Nano | ATmega328P | 5V |
| Arduino Mega 2560 | ATmega2560 | 5V |
| Arduino Leonardo | ATmega32U4 | 5V |
| Arduino Due | SAM3X8E | 3.3V |
| Arduino Zero | SAMD21 | 3.3V |
| Nano 33 IoT | SAMD21 + NINA-W10 | 3.3V |
| ESP32 Family | Xtensa/RISC-V | 3.3V |
| ESP8266 (NodeMCU, Wemos D1 mini) | Xtensa | 3.3V |

> **Core has no mandatory third-party dependencies.** Optional drivers (displays, DHT, Servo) are guarded so they compile even when the library is absent.

## Installation

### Arduino IDE (.zip)

1. Download this repo as `.zip` or grab the release `AARUARDUINO-1.0.0.zip`
2. Arduino IDE → **Sketch** → **Include Library** → **Add .ZIP Library** → select the file
3. Verify: **File** → **Examples** → **AARUARDUINO** → **Basic**

### Arduino CLI

```bash
arduino-cli lib install --zip-path AARUARDUINO-1.0.0.zip
```

## Quick Start

```cpp
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.ultrasonic(7, 8);   // Uno pins; ESP32 e.g. (5, 18)
  AARUARDUINO.led(13);
  AARUARDUINO.buzzer(9);
  AARUARDUINO.lcdI2C(0x27, 16, 2);
}

void loop() {
  AARUARDUINO.update();
  float cm = AARUARDUINO.readUltrasonic();
  if (AARUARDUINO.lastError() != AARUARDUINO_OK) return;
  
  if (cm > 0 && cm < 20) {
    AARUARDUINO.setLED(true);
    AARUARDUINO.beep(2000, 100);
    AARUARDUINO.displayText("Close!");
  } else {
    AARUARDUINO.setLED(false);
  }
  delay(100);
}
```

### Board-Specific Aliases

```cpp
AARUARDUINO.begin();
ultrasonic_uno(7, 8);
float cm = ultrasonic_uno();
```

## Module Reference

**1136 implemented modules** across all categories:

| Category | Count | Examples |
|----------|:-----:|---------|
| Analog Sensors | 324 | LDR, MQ-x gas, LM35/TMP36/NTC, soil, rain, sound, FSR, current/voltage, pH/EC/ORP/TDS, dust/PM |
| Digital Sensors | 93 | PIR, radar, IR obstacle/line, flame, smoke, touch, reed, hall, limit, vibration, tilt, shock, pulse/flow, keypad, button |
| I2C Devices | 164 | MPU6050/9250, BNO055, BME280/680, BMP280/388, SHT3x/40, AHT10/20, TSL2561, VEML7700, APDS9960, VL53L0X, SCD30/41, SGP30/40, DS3231, PCF8523, EEPROM/FRAM, MCP23017, PCA9685, ADS1115, INA219/226, SSD1306, SH1106, LCD1602/2004, HT16K33 |
| SPI Devices | 32 | MAX7219, ILI9341/ST7735/ST7789/HX8357 TFT, SSD1351/1331 OLED, ePaper, MFRC522/PN532 RFID, W25Q/GD25 flash, SD card, MCP3008/3208 ADC, MCP4822 DAC, TMC5160 stepper |
| UART Devices | 18 | GPS NEO-6M/7M/8M/M8N/M9N, MH-Z19 CO2, PZEM-004T/017, DFPlayer/YX5200/JQ6500 MP3, PMS5003/7003/SDS011, HLK-LD2410/2450/MR60BHA1, PN532 NFC, ZE-series gas |
| 1-Wire Devices | 15 | DS18B20/S20/22/25, DS2438/2413/2406/2408/2450/2417/2423, iButton |
| Actuators | 97 | LED, buzzer, relay 1-32CH, SSR, solenoid, laser, RGB, PWM, DC motor, servo SG90/MG90S/MG996R/DS3218, stepper A4988/DRV8825/TB6600/DM542/TMC2208/28BYJ-48/NEMA, fans, pumps, heaters, Peltier |
| Displays | 74 | TM1637, MAX7219, WS2812/SK6812/APA102/SK9822/WS2801 strips, ePaper 1.54"-7.5", SS1351/1331 OLED |
| Radio | 89 | IR NEC/Sony/RC5/RC6, ESP32/ESP8266 WiFi, BLE, LoRa Ra-02/E22/RFM95/SX1262, RF433/315, NRF24L01, RFM69 |
| Storage | 115 | SD/microSD/SDIO, W25Q/GD25/EN25/AT25/MX25 flash, EEPROM 24LC/25AA/25LC/AT25, FRAM MB85RC/FM25 |
| Special/Composite | 115 | Joystick, rotary encoder, potentiometer, shields, GPS/GNSS, GSM/NB-IoT/LTE modems |

Full module list: [`docs/modules.md`](docs/modules.md)

## Safety Warnings

> **Voltage:** ESP32/ESP8266 GPIO is **3.3V, not 5V tolerant**. HC-SR04 echo is 5V — use a voltage divider (1k/2k) or level shifter.

> **Current:** Never drive motors, relays, servos, or high-current loads directly from GPIO. Use driver modules + external power.

> **Relays:** Can be active-HIGH or active-LOW: `AARUARDUINO.relay(pin, activeHigh)`.

## Verification

No Arduino compiler required to validate:

```bash
python tests/host/test_aaruarduino.py    # Pure-math host tests (36/36 pass)
python tools/validate_registry.py         # 1136 modules, 1000+ target met
python tools/gen_keywords.py              # keywords.txt for Arduino IDE
python tools/gen_module_docs.py           # docs/modules.md
python tests/validate/validate.py         # Structure & branding checks
```

## Documentation

| Document | Description |
|----------|-------------|
| [Module Reference](docs/modules.md) | Full 1136-module listing with bus, API, units, boards |
| [API Reference](docs/api-reference.md) | Complete function list, units, error codes |
| [Board Compatibility](docs/board-compatibility.md) | Compatibility matrix for all supported boards |
| [Pin Reference](docs/pin-reference.md) | Safe pin assignments per board |
| [Wiring Guide](docs/wiring-guide.md) | Wiring diagrams and voltage divider circuits |
| [Troubleshooting](docs/troubleshooting.md) | Common failures and solutions |
| [Architecture](docs/architecture.md) | Internal design and module system |
| [Adding a Module](docs/adding-a-module.md) | How to extend the library |

## License

**MIT** — see [LICENSE](LICENSE).

---

<div align="center">

**AARUARDUINO v1.0.0** — Made by [AARUTO](https://github.com/AARUTO347)

[GitHub](https://github.com/AARUTO347/AARUARDUINO) • [Releases](https://github.com/AARUTO347/AARUARDUINO/releases) • [Issues](https://github.com/AARUTO347/AARUARDUINO/issues)

</div>
