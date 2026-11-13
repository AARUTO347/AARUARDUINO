#!/usr/bin/env python3
"""Report class members that have no default initializer.

Members of the library's own classes are skipped (they initialise
themselves), and nested aggregate members are covered by the aggregate's own
initializer.

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
    "Servo", "Stream", "HardwareSerial", "SoftwareSerial",
}

problems = []
for root, dirs, files in os.walk(SRC):
    for f in sorted(files):
        if not f.endswith(".h"):
            continue
        p = os.path.join(root, f)
        raw = open(p, encoding="utf-8", errors="replace").read()
        s = re.sub(r"//[^\n]*", "", raw)
        s = re.sub(r"/\*.*?\*/", "", s, flags=re.S)

        for cm in re.finditer(r"\bclass\s+([A-Za-z_]\w*)\s*(?:final\s*)?(?::[^{]*)?\{", s):
            cname = cm.group(1)
            i = cm.end() - 1
            depth = 0
            end = len(s)
            while i < len(s):
                if s[i] == "{":
                    depth += 1
                elif s[i] == "}":
                    depth -= 1
                    if depth == 0:
                        end = i
                        break
                i += 1
            body = s[cm.end():end]

            # Walk the class body tracking brace depth so that only members
            # declared directly in the class are inspected.
            d = 0
            for line in body.split("\n"):
                line = line.strip()
                if not line:
                    continue
                if d == 0:
                    decl = line[:-1].strip() if line.endswith(";") else None
                    if decl and "(" not in decl and ")" not in decl \
                            and "=" not in decl \
                            and not decl.startswith(("}", "typedef", "using", "friend",
                                                     "public", "private", "protected")):
                        # Skip members that are objects of the library's own
                        # classes (they initialise themselves).
                        toks = [t for t in decl.split()
                                if t not in ("class", "struct", "const", "volatile", "static")]
                        if toks and toks[0] in OBJECT_TYPES:
                            continue
                        if any(t in OBJECT_TYPES for t in toks):
                            continue
                        problems.append((f, cname, decl))
                d += line.count("{") - line.count("}")

if problems:
    print(f"UNINITIALIZED MEMBERS ({len(problems)}):")
    for f, c, d in problems:
        print(f"  {f:28} {c:22} {d}")
    sys.exit(1)
print("OK - all class members have default initializers")
