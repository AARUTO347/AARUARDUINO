# Contributing

MADE BY AARUTO. See `docs/adding-a-module.md` for how to add a module. All contributions must pass the validation suite:

```sh
python tools/generate_registry.py
python tools/validate_registry.py
python tools/gen_keywords.py
python tools/gen_module_docs.py
python tests/validate/validate.py
python tests/host/test_aaruarduino.py
```

All checks must pass before a pull request is merged.
