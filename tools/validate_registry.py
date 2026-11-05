#!/usr/bin/env python3
"""
AARUARDUINO Registry Validator
MADE BY AARUTO
Validates modules.csv: count (>=1000), uniqueness, API coverage, required fields.
Run: python tools/validate_registry.py
"""
import csv
import sys
import os

REGISTRY_PATH = os.path.join(os.path.dirname(__file__), '..', 'registry', 'modules.csv')
SRC_PATH = os.path.join(os.path.dirname(__file__), '..', 'src')
EXAMPLES_PATH = os.path.join(os.path.dirname(__file__), '..', 'examples')

REQUIRED_FIELDS = [
    'id', 'name', 'category', 'bus', 'api_function', 'unit',
    'board_min', 'dependency', 'example', 'status', 'description'
]

VALID_CATEGORIES = {
    'ANALOG', 'DIGITAL', 'I2C', 'SPI', 'UART', 'ONEWIRE',
    'PWM', 'DISPLAY', 'ACTUATOR', 'RADIO', 'STORAGE', 'SPECIAL'
}

VALID_BUSES = {
    'GPIO', 'ANALOG', 'I2C', 'SPI', 'UART', '1-WIRE', 'ONEWIRE', '2-WIRE',
    'STEP/DIR', '4-WIRE', 'QUAD', 'PULSE', 'I2S', 'SDIO', 'PWM', 'SPECIAL'
}

VALID_STATUSES = {'IMPLEMENTED', 'STUB', 'PLANNED'}


def load_registry():
    with open(REGISTRY_PATH, 'r', encoding='utf-8') as f:
        return list(csv.DictReader(f))


def check_required_fields(rows):
    errors = []
    for i, row in enumerate(rows, 1):
        for field in REQUIRED_FIELDS:
            if field not in row or not row[field].strip():
                errors.append(f"Row {i} ({row.get('id','?')}): missing field '{field}'")
    return errors


def check_unique_ids(rows):
    seen = {}
    errors = []
    for i, row in enumerate(rows, 1):
        id_ = row['id'].strip()
        if id_ in seen:
            errors.append(f"Duplicate ID '{id_}' at row {i} (first at row {seen[id_]})")
        seen[id_] = i
    return errors


def check_categories(rows):
    errors = []
    for i, row in enumerate(rows, 1):
        if row['category'].strip().upper() not in VALID_CATEGORIES:
            errors.append(f"Row {i} ({row['id']}): invalid category '{row['category']}'")
    return errors


def check_buses(rows):
    errors = []
    for i, row in enumerate(rows, 1):
        if row['bus'].strip().upper() not in VALID_BUSES:
            errors.append(f"Row {i} ({row['id']}): invalid bus '{row['bus']}'")
    return errors


def check_statuses(rows):
    errors = []
    for i, row in enumerate(rows, 1):
        if row['status'].strip().upper() not in VALID_STATUSES:
            errors.append(f"Row {i} ({row['id']}): invalid status '{row['status']}'")
    return errors


def check_api_in_headers(rows):
    errors = []
    header_text = ''
    for root, dirs, files in os.walk(SRC_PATH):
        for f in files:
            if f.endswith('.h'):
                try:
                    with open(os.path.join(root, f), 'r', encoding='utf-8') as fh:
                        header_text += fh.read() + '\n'
                except Exception:
                    pass
    for i, row in enumerate(rows, 1):
        api = row['api_function'].strip()
        if api and api not in header_text:
            errors.append(f"Row {i} ({row['id']}): api_function '{api}' not found in any .h header")
    return errors


def count_implemented(rows):
    return sum(1 for r in rows if r['status'].strip().upper() == 'IMPLEMENTED')


def count_by_category(rows):
    counts = {}
    for r in rows:
        if r['status'].strip().upper() == 'IMPLEMENTED':
            cat = r['category'].strip().upper()
            counts[cat] = counts.get(cat, 0) + 1
    return counts


def count_by_bus(rows):
    counts = {}
    for r in rows:
        if r['status'].strip().upper() == 'IMPLEMENTED':
            bus = r['bus'].strip().upper()
            counts[bus] = counts.get(bus, 0) + 1
    return counts


def main():
    print("AARUARDUINO Registry Validator")
    print("=" * 50)
    rows = load_registry()
    print(f"Total rows: {len(rows)}")

    all_errors = []
    all_errors += check_required_fields(rows)
    all_errors += check_unique_ids(rows)
    all_errors += check_categories(rows)
    all_errors += check_buses(rows)
    all_errors += check_statuses(rows)
    all_errors += check_api_in_headers(rows)

    impl = count_implemented(rows)
    print(f"\nImplemented modules: {impl}")
    if impl >= 1000:
        print("[OK] TARGET MET: 1000+ modules implemented!")
    else:
        print("[FAIL] TARGET NOT MET: need %d more modules" % (1000 - impl))

    print("\nBy Category:")
    for cat, cnt in sorted(count_by_category(rows).items()):
        print(f"  {cat}: {cnt}")
    print("\nBy Bus:")
    for bus, cnt in sorted(count_by_bus(rows).items()):
        print(f"  {bus}: {cnt}")

    if all_errors:
        print(f"\nERRORS ({len(all_errors)}):")
        for e in all_errors:
            print(f"  - {e}")
        return 1
    print("[OK] All validation checks passed!")
    return 0


if __name__ == '__main__':
    sys.exit(main())