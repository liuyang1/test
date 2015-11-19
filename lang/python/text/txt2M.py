#! /usr/bin/env python3

"""
transfer text novel for mobile reading

remove extra newline, join line to one long line.
"""

import sys

fn = sys.argv[1]
with open(fn) as fp:
    lines = []
    for line in fp.readlines():
        line = line.strip()
        if line == "":
            print(" ".join(lines) + "\n")
            lines = []
        else:
            lines.append(line)
