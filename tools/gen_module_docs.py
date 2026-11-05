#!/usr/bin/env python3
"""
AARUARDUINO Module Documentation Generator
MADE BY AARUTO
Generates docs/modules.md from registry.
Run: python tools/gen_module_docs.py
"""
import csv
import os

REGISTRY_PATH = os.path.join(os.path.dirname(__file__), '..', 'registry', 'modules.csv')
OUTPUT_PATH = os.path.join(os.path.dirname(__file__), '..', 'docs', 'modules.md')

CATEGORY_NAMES = {
    'ANALOG': 'Analog Sensors (1-pin)',
    'DIGITAL': 'Digital Sensors (1-pin)',
    'I2C': 'I²C Devices',
    'SPI': 'SPI Devices',
    'UART': 'UART / Serial Devices',
    'ONEWIRE': '1-Wire Devices',
    'PWM': 'PWM Outputs',
    'DISPLAY': 'Displays',
    'ACTUATOR': 'Actuators',
    'RADIO': 'Radio / Wireless',
    'STORAGE': 'Storage',
    'SPECIAL': 'Special / Composite',
}

BUS_NAMES = {
    'GPIO': 'Digital GPIO', 'ANALOG': 'Analog Input', 'I2C': 'I²C',
    'SPI': 'SPI', 'UART': 'UART/Serial', '1-WIRE': '1-Wire', '2-WIRE': '2-Wire',
    'STEP/DIR': 'Step/Direction', '4-WIRE': '4-Wire Unipolar', 'QUAD': 'Quadrature',
    'PULSE': 'Pulse Timing', 'I2S': 'I²S Audio', 'SDIO': 'SDIO',
}


def load_registry():
    with open(REGISTRY_PATH, 'r', encoding='utf-8') as f:
        return list(csv.DictReader(f))


def main():
    rows = load_registry()
    impl = [r for r in rows if r['status'].strip().upper() == 'IMPLEMENTED']
    with open(OUTPUT_PATH, 'w', encoding='utf-8') as f:
        f.write("# AARUARDUINO Module Reference\n")
        f.write("## MADE BY AARUTO\n\n")
        f.write(f"**Total Implemented Modules: {len(impl)}**\n\n---\n\n")
        for cat_code, cat_name in CATEGORY_NAMES.items():
            cat_rows = [r for r in impl if r['category'].strip().upper() == cat_code]
            if not cat_rows:
                continue
            f.write(f"## {cat_name} ({len(cat_rows)} modules)\n\n")
            f.write("| ID | Module | Bus | API Function | Unit | Boards | Dependency | Example |\n")
            f.write("|----|--------|-----|--------------|------|--------|------------|---------|\n")
            for row in sorted(cat_rows, key=lambda x: x['id']):
                bus_name = BUS_NAMES.get(row['bus'].strip().upper(), row['bus'])
                dep = row['dependency'].strip()
                if not dep or dep == 'None':
                    dep = '—'
                f.write(f"| {row['id']} | {row['name']} | {bus_name} | "
                        f"`{row['api_function']}` | {row['unit']} | {row['board_min']} | {dep} | "
                        f"[{row['example']}](../examples/{row['example']}) |\n")
            f.write("\n---\n\n")
        f.write("## Summary by Category\n\n| Category | Count |\n|----------|-------|\n")
        for cat_code, cat_name in CATEGORY_NAMES.items():
            cnt = len([r for r in impl if r['category'].strip().upper() == cat_code])
            if cnt > 0:
                f.write(f"| {cat_name} | {cnt} |\n")
        f.write("\n## Summary by Bus\n\n| Bus | Count |\n|-----|-------|\n")
        bus_counts = {}
        for r in impl:
            bus = r['bus'].strip().upper()
            bus_counts[bus] = bus_counts.get(bus, 0) + 1
        for bus, cnt in sorted(bus_counts.items(), key=lambda x: -x[1]):
            f.write(f"| {BUS_NAMES.get(bus, bus)} | {cnt} |\n")
    print(f"Generated {OUTPUT_PATH} with {len(impl)} implemented modules")


if __name__ == '__main__':
    main()