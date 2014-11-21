import itertools
import sys


def genPt(area):
    x = range(area[0], area[2] + 1)
    y = range(area[1], area[3] + 1)
    return list(itertools.product(x, y))


def bestPos2(arealst):
    pt = []
    for area in arealst:
        pt.extend(genPt(area))
    dist = sys.maxint
    for x, y in pt:
        v = sum([abs(x - tx) + abs(y - ty) for tx, ty in pt])
        if v < dist:
            dist = v
            hlst = [(x, y)]
        elif v == dist:
            hlst.append((x, y))
    hlst.sort()
    x, y = hlst[0]
    return (x, y, dist)


def testcase(seq):
    n = int(raw_input())
    arealst = []
    for i in range(n):
        l = raw_input()
        area = [int(i) for i in l.split()]
        arealst.append(area)
    x, y, dist = bestPos2(arealst)
    print "Case #%d: %d %d %d" % (seq, x, y, dist)


if __name__ == "__main__":
    n = int(raw_input())
    for i in range(n):
        testcase(i + 1)
