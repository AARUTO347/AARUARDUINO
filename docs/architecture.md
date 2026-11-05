# AARUARDUINO Architecture
## MADE BY AARUTO

- `src/AARUARDUINO.h/.cpp` — public singleton `AARUARDUINO`, delegates to modules.
- `src/AARUARDUINOTypes.h` — errors, families, version, categories.
- `src/AARUARDUINOConfig.h` — timeouts, debounce, opt-out macros.
- `src/AARUARDUINOBoard.h` — compile-time detection + pin validation.
- `src/engines/` — shared backends (Analog, Digital, PWM, I2C, SPI, UART, OneWire, Pulse).
- `src/drivers/` — per-device drivers (HCSR04, DHT, MPU6050, SSD1306, etc.).
- `src/utility/AaruarduinoMath.h` — pure header math, host-testable.
- `registry/modules.csv` — source of truth for the 1000+ module list.
- `tools/` — registry generator, validator, keywords/docs generators.
- `tests/` — host tests (Python) + structure validator.
- `examples/` — one sketch per major module family.

Rules: no heap in core paths, no blocking in `update()`, headers idempotent, never fake data (failures return NaN/-1/false + error code), optional deps guarded with `__has_include`.
