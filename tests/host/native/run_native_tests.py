#!/usr/bin/env python3
"""
AARUARDUINO Native Test Runner
MADE BY AARUTO

Builds and runs the native C++ host tests (no Arduino toolchain required).
Falls back to a skip message when no host C++ compiler is available.

Run: python tests/host/native/run_native_tests.py
"""
import os
import shutil
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..', '..'))
SRC = os.path.join(ROOT, 'src')
OUT = os.path.join(ROOT, 'build', 'native')

TESTS = [
    {
        'name': 'gps',
        'src': os.path.join(HERE, 'test_gps.cpp'),
        'libs': [
            os.path.join(SRC, 'drivers', 'AaruGPS.cpp'),
            os.path.join(SRC, 'engines', 'AaruUARTEngine.cpp'),
        ],
    },
    {
        'name': 'ssd1306',
        'src': os.path.join(HERE, 'test_ssd1306.cpp'),
        'libs': [
            os.path.join(SRC, 'drivers', 'AaruSSD1306.cpp'),
            os.path.join(SRC, 'engines', 'AaruI2CEngine.cpp'),
        ],
    },
    {
        'name': 'onewire',
        'src': os.path.join(HERE, 'test_onewire.cpp'),
        'libs': [
            os.path.join(SRC, 'engines', 'AaruOneWireEngine.cpp'),
        ],
    },
    {
        'name': 'bme280',
        'src': os.path.join(HERE, 'test_bme280.cpp'),
        'libs': [
            os.path.join(SRC, 'drivers', 'AaruBME280.cpp'),
            os.path.join(SRC, 'engines', 'AaruI2CEngine.cpp'),
        ],
    },
]


def find_compiler():
    for exe in ('g++', 'clang++', 'c++'):
        path = shutil.which(exe)
        if path:
            return path
    return None


def main():
    cc = find_compiler()
    if not cc:
        print("SKIP: no host C++ compiler (g++ / clang++) found on PATH")
        return 0

    os.makedirs(OUT, exist_ok=True)
    failures = 0

    for t in TESTS:
        exe = os.path.join(OUT, f"test_{t['name']}.exe" if os.name == 'nt' else f"test_{t['name']}")
        cmd = [cc, '-std=c++11', '-Wall',
               '-I', SRC,
               '-I', os.path.join(SRC, 'engines'),
               '-I', os.path.join(SRC, 'drivers'),
               '-o', exe, t['src']] + t['libs']
        print(f"--- building native test '{t['name']}' ---")
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode != 0:
            failures += 1
            print(r.stdout)
            print(r.stderr)
            print(f"FAILED to build '{t['name']}'")
            continue

        r = subprocess.run([exe], capture_output=True, text=True)
        print(r.stdout.strip())
        if r.returncode != 0:
            failures += 1
            print(f"NATIVE TEST '{t['name']}' FAILED")
        print()

    if failures:
        print(f"RESULT: {failures} native test group(s) failed")
        return 1
    print("RESULT: all native tests passed")
    return 0


if __name__ == '__main__':
    sys.exit(main())
