# Adding a Module to AARUARDUINO
## MADE BY AARUTO

1. Add a row to `registry/modules.csv` with id, name, category, bus, api_function, unit, board_min, dependency, example, status=IMPLEMENTED, description.
2. If a new API function is needed, add it to `src/AARUARDUINO.h` and implement in `src/AARUARDUINO.cpp`.
3. For a new driver, create `src/drivers/AaruX.h` (self-contained class, no globals).
4. Validate pins via `aaruarduinoValidateDigitalPin()`/`aaruarduinoValidateAnalogPin()`/`aaruarduinoValidatePwmPin()`.
5. Guard third-party includes with `#if __has_include(<Lib.h>)`; return `AARUARDUINO_MISSING_DEP` when absent.
6. Guard board-specific code with `AARUARDUINO_BOARD_*` macros; return `AARUARDUINO_UNSUPPORTED` otherwise.
7. Put pure math in `src/utility/AaruarduinoMath.h` and extend `tests/host/test_aaruarduino.py`.
8. Add an example under `examples/` using real API + valid per-board pins.
9. Run: `python tools/generate_registry.py`, `python tools/validate_registry.py`, `python tools/gen_keywords.py`, `python tools/gen_module_docs.py`, `python tests/validate/validate.py`, `python tests/host/test_aaruarduino.py`.
10. All checks must pass before committing.
