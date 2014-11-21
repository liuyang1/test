#! /usr/bin/env python
import math
g = 9.8


def calc(v, d):
    t = d * g / v / v
    if t>1.0:
        t=1.0
    theta = math.asin(t) / 2.0
    return theta/math.pi*180.0


def testcase(seq):
    line=raw_input()
    line=line.split()
    v = float(line[0])
    d = float(line[1])
    print "Case #%d: %f" % (seq, calc(v, d))


if __name__ == "__main__":
    n = int(raw_input())
    for i in range(n):
        testcase(i+1)
