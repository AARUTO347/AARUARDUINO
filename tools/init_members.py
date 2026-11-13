#!/usr/bin/env python3
"""Add default initializers to class members (brace-depth aware, safe).

Only lines that sit directly inside a class body (depth 1 relative to the
class) and that look like a plain data member declaration are touched, so
function bodies and free code are never modified.

MADE BY AARUTO - build tooling
"""
import os
import re
import sys

SRC = r"D:\AARU_system\AARUARDUINO\src"

OBJECT_TYPES = {
    "AaruAnalogEngine", "AaruDigitalEngine", "AaruPWMEngine", "AaruI2CEngine",
    "AaruSPIEngine", "AaruUARTEngine", "AaruOneWireEngine", "AaruPulseEngine",
    "AaruHCSR04", "AaruDHT", "AaruMPU6050", "AaruDS18B20", "AaruDS3231",
    "AaruBME280", "AaruBH1750", "AaruSSD1306", "AaruLCD", "AaruMAX7219",
    "AaruTM1637", "AaruWS2812", "AaruGPS", "AaruIRRemote", "AaruStepper",
    "Servo", "Stream", "HardwareSerial", "SoftwareSerial", "Wire", "SPI",
}

INT_TYPES = {
    "uint8_t", "uint16_t", "uint32_t", "uint64_t", "int8_t", "int16_t",
    "int32_t", "int64_t", "unsigned", "signed", "int", "long", "short",
    "char", "size_t", "byte", "unsigned long", "unsigned int", "unsigned short",
    "signed char", "long long", "unsigned long long",
}

DEFAULT_FOR = {
    "AaruarduinoError": "AARUARDUINO_OK",
    "AaruAnalogKind": "AARU_AN_KIND_NONE",
}

# A member declaration: indentation, a type, then a comma separated name list.
DECL_RE = re.compile(
    r"^(?P<indent>[ \t]+)"
    r"(?P<body>(?:volatile\s+|const\s+|static\s+)*"
    r"(?:class\s+|struct\s+)?"
    r"(?:(?:unsigned|signed|long|short|int|char|float|double|bool|void)\s+)*"
    r"[A-Za-z_][A-Za-z0-9_:]*)"
    r"(?P<ptr>\s*\**)\s+"
    r"(?P<names>[A-Za-z_]\w*(?:\s*\[[^\]]*\])?"
    r"(?:\s*,\s*[A-Za-z_]\w*(?:\s*\[[^\]]*\])?)*)\s*;\s*$"
)

CODE_KEYWORDS = {
    "return", "if", "for", "while", "switch", "case", "break", "continue",
    "else", "goto", "do", "typedef", "using", "extern", "template",
}


def default_for(decl_type, has_array):
    if has_array:
        return "{}"
    t = " ".join(decl_type.split())
    bare = t.split()[-1] if t.split() else ""
    if bare in OBJECT_TYPES:
        return None
    if "*" in t:
        return "nullptr"
    if t in DEFAULT_FOR:
        return DEFAULT_FOR[t]
    if bare in DEFAULT_FOR:
        return DEFAULT_FOR[bare]
    if bare == "bool":
        return "false"
    if bare == "float":
        return "0.0f"
    if bare == "double":
        return "0.0"
    if t in INT_TYPES or bare in INT_TYPES:
        return "0"
    # Unknown type -> POD aggregate (struct member)
    return "{}"


def process(path):
    lines = open(path, encoding="utf-8", errors="replace").read().split("\n")
    out = []
    depth = 0
    in_class = False
    changed = 0

    for line in lines:
        stripped = line.strip()
        m = DECL_RE.match(line)

        if in_class and depth == 1 and m and "=" not in line:
            first = stripped.split()[0] if stripped.split() else ""
            if first not in CODE_KEYWORDS:
                init = default_for(m.group("body") + m.group("ptr"),
                                   "[" in m.group("names"))
                if init is not None:
                    names = [n.strip() for n in m.group("names").split(",")]
                    decls = ", ".join(f"{n} = {init}" for n in names)
                    out.append(f"{m.group('indent')}{m.group('body')}"
                               f"{m.group('ptr')} {decls};")
                    changed += 1
                    continue

        out.append(line)

        if not in_class:
            if re.search(r"\bclass\s+[A-Za-z_]\w*\s*(?:final\s*)?(?::[^{]*)?\{", line):
                in_class = True
                depth = 1
        else:
            depth += line.count("{") - line.count("}")
            if depth <= 0:
                in_class = False
                depth = 0

    if changed:
        open(path, "w", encoding="utf-8", newline="\n").write("\n".join(out))
    return changed


total = 0
for root, dirs, files in os.walk(SRC):
    for f in sorted(files):
        if not f.endswith(".h"):
            continue
        p = os.path.join(root, f)
        n = process(p)
        if n:
            print(f"  {f:26} {n}")
            total += n
print(f"TOTAL: {total}")
