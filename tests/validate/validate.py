#!/usr/bin/env python3
"""
AARUARDUINO Structure Validator
MADE BY AARUTO
Validates source structure, branding, includes, examples.
Run: python tests/validate/validate.py
"""
import os
import sys

SRC_PATH = os.path.join(os.path.dirname(__file__), '..', '..', 'src')
EXAMPLES_PATH = os.path.join(os.path.dirname(__file__), '..', '..', 'examples')
REGISTRY_PATH = os.path.join(os.path.dirname(__file__), '..', '..', 'registry', 'modules.csv')

REQUIRED_HEADERS = [
    'AARUARDUINO.h', 'AARUARDUINOTypes.h', 'AARUARDUINOConfig.h',
    'AARUARDUINOBoard.h', 'AARUARDUINO.cpp'
]

REQUIRED_ENGINES = [
    'AaruAnalogEngine.h', 'AaruDigitalEngine.h', 'AaruPWMEngine.h',
    'AaruI2CEngine.h', 'AaruSPIEngine.h', 'AaruUARTEngine.h',
    'AaruOneWireEngine.h', 'AaruPulseEngine.h'
]

REQUIRED_DRIVERS = [
    'AaruHCSR04.h', 'AaruDHT.h', 'AaruMPU6050.h', 'AaruDS18B20.h',
    'AaruDS3231.h', 'AaruBME280.h', 'AaruBH1750.h', 'AaruSSD1306.h',
    'AaruLCD.h', 'AaruMAX7219.h', 'AaruTM1637.h', 'AaruWS2812.h',
    'AaruGPS.h', 'AaruIRRemote.h', 'AaruStepper.h'
]

REQUIRED_UTILITY = ['AaruarduinoMath.h']

BRANDING_STRING = "MADE BY AARUTO"


def check_file_exists(path, name):
    full = os.path.join(path, name)
    return None if os.path.exists(full) else f"Missing file: {name}"


def check_branding_in_file(path, name):
    full = os.path.join(path, name)
    try:
        with open(full, 'r', encoding='utf-8') as f:
            content = f.read()
        if BRANDING_STRING not in content:
            return f"Missing branding '{BRANDING_STRING}' in {name}"
    except Exception as e:
        return f"Error reading {name}: {e}"
    return None


def check_include_guards(path, name):
    full = os.path.join(path, name)
    if not name.endswith('.h'):
        return None
    try:
        with open(full, 'r', encoding='utf-8') as f:
            content = f.read()
        lines = [l for l in content.splitlines() if l.strip()]
        if len(lines) < 2:
            return f"File too short: {name}"
        # Guard may follow a comment banner (standard Arduino style)
        if '#ifndef' not in content:
            return f"Missing #ifndef guard in {name}"
        if '#define' not in content:
            return f"Missing #define guard in {name}"
        last_nonempty = lines[-1]
        if not last_nonempty.strip().startswith('#endif'):
            return f"Missing #endif in {name}"
        import re
        m = re.search(r'#ifndef\s+(\w+)', content)
        if m and m.group(1) not in last_nonempty:
            return f"Guard mismatch in {name}"
    except Exception as e:
        return f"Error checking guards in {name}: {e}"
    return None


def check_singleton_consistency():
    errors = []
    try:
        with open(os.path.join(SRC_PATH, 'AARUARDUINO.h'), 'r', encoding='utf-8') as f:
            h_content = f.read()
        with open(os.path.join(SRC_PATH, 'AARUARDUINO.cpp'), 'r', encoding='utf-8') as f:
            cpp_content = f.read()
    except Exception as e:
        return [f"Error reading singleton files: {e}"]
    for engine in REQUIRED_ENGINES:
        if engine.replace('.h', '') not in h_content:
            errors.append(f"Engine {engine} not declared in AARUARDUINO.h")
    for driver in REQUIRED_DRIVERS:
        if driver.replace('.h', '') not in h_content:
            errors.append(f"Driver {driver} not declared in AARUARDUINO.h")
    for engine in REQUIRED_ENGINES:
        inc = f'#include "engines/{engine}"'
        if inc not in cpp_content and inc not in h_content:
            errors.append(f"Missing #include engines/{engine} in AARUARDUINO.h/.cpp")
    for driver in REQUIRED_DRIVERS:
        inc = f'#include "drivers/{driver}"'
        if inc not in cpp_content and inc not in h_content:
            errors.append(f"Missing #include drivers/{driver} in AARUARDUINO.h/.cpp")
    return errors


def check_examples():
    errors = []
    if not os.path.exists(EXAMPLES_PATH):
        return ["Examples directory not found"]
    for ex_dir in os.listdir(EXAMPLES_PATH):
        ex_path = os.path.join(EXAMPLES_PATH, ex_dir)
        if not os.path.isdir(ex_path):
            continue
        ino_files = [f for f in os.listdir(ex_path) if f.endswith('.ino')]
        if not ino_files:
            errors.append(f"Example {ex_dir}: no .ino file")
            continue
        full_ino = os.path.join(ex_path, ino_files[0])
        try:
            with open(full_ino, 'r', encoding='utf-8') as f:
                content = f.read()
            if '#include <AARUARDUINO.h>' not in content:
                errors.append(f"Example {ex_dir}: missing #include <AARUARDUINO.h>")
            if 'AARUARDUINO.begin()' not in content:
                errors.append(f"Example {ex_dir}: missing AARUARDUINO.begin()")
            if 'void setup()' not in content:
                errors.append(f"Example {ex_dir}: missing setup()")
            if 'void loop()' not in content:
                errors.append(f"Example {ex_dir}: missing loop()")
        except Exception as e:
            errors.append(f"Error reading example {ex_dir}: {e}")
    return errors


def check_library_properties():
    prop_path = os.path.join(os.path.dirname(__file__), '..', '..', 'library.properties')
    errors = []
    if not os.path.exists(prop_path):
        return ["Missing library.properties"]
    try:
        with open(prop_path, 'r', encoding='utf-8') as f:
            content = f.read()
        for req in ['name=', 'version=', 'author=', 'sentence=', 'paragraph=', 'category=', 'url=', 'architectures=', 'includes=']:
            if req not in content:
                errors.append(f"library.properties missing '{req}'")
        if 'AARUTO' not in content:
            errors.append("library.properties missing author AARUTO")
    except Exception as e:
        errors.append(f"Error reading library.properties: {e}")
    return errors


def check_readme():
    readme_path = os.path.join(os.path.dirname(__file__), '..', '..', 'README.md')
    errors = []
    if not os.path.exists(readme_path):
        return ["Missing README.md"]
    try:
        with open(readme_path, 'r', encoding='utf-8') as f:
            content = f.read()
        if 'AARUARDUINO' not in content:
            errors.append("README.md missing AARUARDUINO name")
        if 'MADE BY AARUTO' not in content:
            errors.append("README.md missing MADE BY AARUTO")
        if '1000+' not in content:
            errors.append("README.md should mention 1000+ modules")
    except Exception as e:
        errors.append(f"Error reading README.md: {e}")
    return errors


def main():
    print("AARUARDUINO Structure Validator")
    print("=" * 50)
    all_errors = []
    for h in REQUIRED_HEADERS:
        err = check_file_exists(SRC_PATH, h)
        if err:
            all_errors.append(err)
        else:
            err = check_branding_in_file(SRC_PATH, h)
            if err:
                all_errors.append(err)
            err = check_include_guards(SRC_PATH, h)
            if err:
                all_errors.append(err)
    engines_path = os.path.join(SRC_PATH, 'engines')
    for e in REQUIRED_ENGINES:
        err = check_file_exists(engines_path, e)
        if err:
            all_errors.append(err)
        else:
            err = check_branding_in_file(engines_path, e)
            if err:
                all_errors.append(err)
            err = check_include_guards(engines_path, e)
            if err:
                all_errors.append(err)
    drivers_path = os.path.join(SRC_PATH, 'drivers')
    for d in REQUIRED_DRIVERS:
        err = check_file_exists(drivers_path, d)
        if err:
            all_errors.append(err)
        else:
            err = check_branding_in_file(drivers_path, d)
            if err:
                all_errors.append(err)
            err = check_include_guards(drivers_path, d)
            if err:
                all_errors.append(err)
    util_path = os.path.join(SRC_PATH, 'utility')
    for u in REQUIRED_UTILITY:
        err = check_file_exists(util_path, u)
        if err:
            all_errors.append(err)
        else:
            err = check_branding_in_file(util_path, u)
            if err:
                all_errors.append(err)
            err = check_include_guards(util_path, u)
            if err:
                all_errors.append(err)
    all_errors += check_singleton_consistency()
    all_errors += check_examples()
    all_errors += check_library_properties()
    all_errors += check_readme()
    all_errors += check_static_analysis()
    print("\n" + "=" * 50)
    if all_errors:
        print(f"FAILED: {len(all_errors)} error(s)")
        for e in all_errors:
            print(f"  - {e}")
        return 1
    print("PASSED: All structure checks OK")
    return 0


def check_static_analysis():
    """Every declared method must have a definition, and every member an
    initializer. Both checks are implemented as standalone tools so they can
    also be run on their own."""
    import subprocess
    errors = []
    root = os.path.join(os.path.dirname(__file__), '..', '..')
    for tool, label in (('tools/check_definitions.py', 'missing method definitions'),
                        ('tools/check_init.py', 'members without default initializers')):
        script = os.path.join(root, tool)
        if not os.path.exists(script):
            errors.append(f"Static analysis tool missing: {tool}")
            continue
        r = subprocess.run([sys.executable, script], capture_output=True, text=True)
        if r.returncode != 0:
            out = (r.stdout or '') + (r.stderr or '')
            errors.append(f"{label}: see output of {tool}\n{out.strip()}")
    return errors


if __name__ == '__main__':
    sys.exit(main())