# AARUARDUINO Troubleshooting
## MADE BY AARUTO

## Library not found in Arduino IDE

- Install via Sketch → Include Library → Add .ZIP Library, selecting `AARUARDUINO-1.0.0.zip`.
- Restart the Arduino IDE after installing.
- Confirm `File → Examples → AARUARDUINO → Basic` appears.

## Compilation errors

- `'AARUARDUINO' was not declared` — you forgot `#include <AARUARDUINO.h>`.
- Board not in `architectures` — the library supports `avr,megaavr,renesas_uno,samd,esp32,esp8266,*`. Select the correct board in Tools → Board.
- Missing optional dependency (e.g. `SdFat.h` for SD examples) — examples that need optional libs are guarded; remove or install the dep.

## `lastError()` returns non-OK

Call `AARUARDUINO.errorString()` for a message. Common causes:
- `INVALID_PIN` — pin not valid for this board/operation (see `docs/pin-reference.md`).
- `TIMEOUT` — ultrasonic echo never returned; check wiring/divider.
- `NOT_INITIALIZED` — call `begin()` first, or configure the module before reading.
- `UNSUPPORTED` — feature not available on this board (e.g. WiFi on Uno).
- `MISSING_DEP` — optional third-party library not installed.

## Readings are NaN / 0

- Sensor not connected or unpowered.
- `begin()` not called for that module.
- DHT: readings must be ≥2 s apart; call `dhtFresh()` / `dhtAgeMs()`.
- Ultrasonic: object out of range (2-400 cm) or echo pin floating.

## I²C device not found

- Run the I²C scan example to list addresses.
- Common addresses: SSD1306 = 0x3C/0x3D, LCD backpack = 0x27/0x3F, BME280 = 0x76/0x77, DS3231 = 0x68, MPU6050 = 0x68.
- Add 4.7k pullups on SDA/SCL.

## ESP32 WiFi / analog conflicts

- ADC2 pins (0,2,4,12-15,25-27) conflict with WiFi. Use ADC1 (32-39) when WiFi is active.
- GPIO 6-11 are flash pins — never use them.

## 5 V on 3.3 V boards

- ESP32/ESP8266 are not 5 V tolerant. Use dividers/level shifters on 5 V sensor outputs (e.g. HC-SR04 echo).

## Still stuck?

Open an issue at https://github.com/AARUTO/AARUARDUINO with your board, wiring, and the output of `AARUARDUINO.errorString()`.
