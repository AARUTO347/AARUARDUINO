# AARUARDUINO — Universal Arduino Library
## MADE BY AARUTO

One consistent API for **1000+ sensors, actuators, displays and comms** across all Arduino IDE boards:

- Arduino Uno R3, Uno R4 Minima, Uno R4 WiFi
- Arduino Nano, Mega 2560, Leonardo, Micro, Due, Zero
- Nano 33 IoT, ESP32 family, ESP8266 (NodeMCU, Wemos D1 mini)

Core has **no mandatory third-party dependencies**. Displays, DHT, Servo and optional drivers are guarded so they compile even when the library is absent.

## Install (Arduino IDE → .zip)

1. Download this repo as `.zip` or use the release `AARUARDUINO-1.0.0.zip`.
2. Arduino IDE → Sketch → Include Library → Add .ZIP Library → select the file.
3. Check: File → Examples → AARUARDUINO → Basic.

## Install (Arduino CLI)

```sh
arduino-cli lib install --zip-path AARUARDUINO-1.0.0.zip
```

## Quick start

```cpp
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.ultrasonic(7, 8);  // Uno pins; ESP32 e.g. (5, 18)
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

## Module reference

`docs/modules.md` lists all **1136 implemented modules** with bus, API function, unit, boards and example:

- 324 analog sensors (LDR, MQ-x gas, LM35/TMP36/NTC, soil, rain, sound, FSR, current/voltage, pH/EC/ORP/TDS, dust/PM)
- 93 digital sensors (PIR, radar, IR obstacle/line, flame, smoke, touch, reed, hall, limit, vibration, tilt, shock, pulse/flow/tach, keypad, button)
- 164 I²C devices (MPU6050/9250, BNO055, BME280/680, BMP280/388, SHT3x/40, AHT10/20, TSL2561/2591, VEML7700, TCS34725, APDS9960, VL53L0X/L1X, SCD30/41, SGP30/40, DS3231/1307, PCF8523, EEPROM/FRAM, MCP23017, PCA9685, ADS1115, INA219/226, SSD1306, SH1106, LCD1602/2004, HT16K33)
- 32 SPI devices (MAX7219, ILI9341/ST7735/ST7789/HX8357 TFT, SSD1351/1331 OLED, ePaper, MFRC522/PN532 RFID, W25Q/GD25 flash, SD card, MCP3008/3208 ADC, MCP4822 DAC, TMC5160 stepper, energy ICs)
- 18 UART devices (GPS NEO-6M/7M/8M/M8N/M9N, MH-Z19 CO2, PZEM-004T/017 power, DFPlayer/YX5200/JQ6500 MP3, PMS5003/7003/SDS011 PM, HLK-LD2410/2450/MR60BHA1 radar, PN532 NFC, ZE-series gas)
- 15 1-Wire devices (DS18B20/S20/22/25, DS2438/2413/2406/2408/2450/2417/2423, iButton)
- 97 actuators (LED, buzzer active/passive, relay 1/2/4/8/16/32CH, SSR, reed, solenoid, laser, RGB, PWM out, DC motor, servo SG90/MG90S/MG996R/DS3218/continuous, stepper A4988/DRV8825/TB6600/DM542/TMC2208/TMC2209/28BYJ-48/NEMA17/23/34, fans, pumps, heaters, Peltier)
- 74 displays (TM1637, MAX7219, WS2812/SK6812/APA102/SK9822/WS2801/TM1814/GS8208 strips, ePaper 1.54"/2.13"/2.9"/4.2"/7.5", SS1351/1331 OLED)
- 89 radio (IR NEC/Sony/RC5/RC6, ESP32/ESP8266 WiFi, BLE, LoRa Ra-02/E22/RFM95/SX1262/SX1276, RF433/315, NRF24L01, RFM69)
- 115 storage (SD/microSD/SDIO, W25Q/GD25/EN25/AT25/MX25 flash, EEPROM 24LC/25AA/25LC/AT25, FRAM MB85RC/FM25)
- 115 special/composite (joystick, rotary encoder, potentiometer, shields, GPS/GNSS modules, GSM/NB-IoT/LTE modems)

## Board-specific aliases

```cpp
AARUARDUINO.begin();
ultrasonic_uno(7, 8);
float cm = ultrasonic_uno();
```

Same units, delegate to the singleton.

## Safety

- ESP32/ESP8266 GPIO is **3.3 V, not 5 V tolerant**. HC-SR04 echo is 5 V → use a voltage divider (1k/2k) or level shifter.
- Never drive motors, relays, servos or high-current loads directly from GPIO. Use driver modules + external power.
- Relays can be active-HIGH or active-LOW: `AARUARDUINO.relay(pin, activeHigh)`.

## Verification

No Arduino compiler is required to validate:

```sh
python tests/host/test_aaruarduino.py      # pure-math host tests (36/36 pass)
python tools/validate_registry.py           # 1136 modules, 1000+ target met
python tools/gen_keywords.py                # keywords.txt for Arduino IDE
python tools/gen_module_docs.py             # docs/modules.md
python tests/validate/validate.py           # structure & branding checks
```

## Docs

- `docs/modules.md` — full 1136-module reference
- `docs/api-reference.md` — full function list, units, errors
- `docs/board-compatibility.md` — compatibility matrix
- `docs/pin-reference.md` — safe pins per board
- `docs/wiring-guide.md` — wiring + divider diagrams
- `docs/troubleshooting.md` — common failures
- `docs/architecture.md`, `docs/adding-a-module.md`

## License

MIT — see `LICENSE`.

**MADE BY AARUTO** — AARUARDUINO v1.0.0# AARUARDUINO  
