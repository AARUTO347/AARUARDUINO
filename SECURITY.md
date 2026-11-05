# Security

MADE BY AARUTO. This library interfaces with physical hardware. Always:

- Verify pin validity before writing GPIO (`isValidDigitalPin`, `isValidAnalogPin`, `isValidPwmPin`).
- Use external power for motors, relays, solenoids, heaters, and pumps.
- Never connect 5 V to ESP32/ESP8266 GPIO without level shifting — they are 3.3 V only.
- Double-check wiring before powering on.
- Common GND between the board and all external power supplies.

Report security concerns to the project maintainer at AARUTO.
