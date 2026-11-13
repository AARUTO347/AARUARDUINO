#!/usr/bin/env python3
"""Check that every method declared in a header is defined in the matching .cpp.

MADE BY AARUTO - build tooling
"""
import os
import re
import sys

SRC = r"D:\AARU_system\AARUARDUINO\src"

# Collect all definitions "Class::method" from .cpp files
defs = set()
cpp_files = []
for root, dirs, files in os.walk(SRC):
    for f in files:
        if f.endswith(".cpp"):
            cpp_files.append(os.path.join(root, f))

text = {}
for p in cpp_files:
    s = open(p, encoding="utf-8", errors="replace").read()
    text[p] = s
    for m in re.finditer(r"\b([A-Za-z_]\w*)\s*::\s*([A-Za-z_]\w*)\s*\(", s):
        defs.add((m.group(1), m.group(2)))

# Parse headers for class declarations and their method prototypes
problems = []
hdr_files = []
for root, dirs, files in os.walk(SRC):
    for f in files:
        if f.endswith(".h"):
            hdr_files.append(os.path.join(root, f))

for p in hdr_files:
    s = open(p, encoding="utf-8", errors="replace").read()
    # find "class Name { ... };" blocks
    for cm in re.finditer(r"\bclass\s+([A-Za-z_]\w*)\s*(?:final\s*)?(?::[^{]*)?\{", s):
        cname = cm.group(1)
        # find matching closing brace
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

        # remove comments
        body = re.sub(r"//[^\n]*", "", body)
        body = re.sub(r"/\*.*?\*/", "", body, flags=re.S)

        # method prototypes: "type name(args);" not ending with '{'
        for pm in re.finditer(r"^[ \t]*(?:virtual\s+|static\s+|inline\s+|explicit\s+)*"
                              r"(?:[A-Za-z_][\w:<>,\s\*&]*?)\b([A-Za-z_]\w*)\s*\(([^;{)]*)\)\s*(?:const\s*)?(?:override\s*)?;",
                              body, flags=re.M):
            mname = pm.group(1)
            if mname in ("if", "for", "while", "switch", "return", "sizeof"):
                continue
            # skip constructors / destructors (no return type) -> handled below
            if (cname, mname) not in defs:
                problems.append((os.path.basename(p), cname, mname, pm.group(0).strip()))

if problems:
    print(f"MISSING DEFINITIONS ({len(problems)}):")
    for f, c, m, sig in problems:
        print(f"  {f}: {c}::{m}   ->  {sig}")
    sys.exit(1)
else:
    print("OK - every declared method has a definition")
    sys.exit(0)
