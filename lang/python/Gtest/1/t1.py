#! /usr/bin/env python
import sys


def search(pairlist,color,obj):
    for aa, bb in pairlist:
        if bb == obj:
            aa, bb = bb, aa
        if aa == obj:
            if bb in color.keys():
                if color[bb] != color[obj]:
                    continue
                else:
                    return False
            else:
                color[bb] = (not color[aa])
                ret = search(pairlist,color,bb)
                if ret == False:
                    return False
                else:
                    continue
    return True


def testcase(seq):
    n = int(raw_input())
    pairlist = []
    for i in range(n):
        pair = sys.stdin.readline()
        pair = pair.split()
        pairlist.append(tuple(pair))
    ret = search(pairlist,{pairlist[0][0]:True},pairlist[0][0])
    if ret:
        print "Case #%d: Yes" % (seq)
    else:
        print "Case #%d: No" % (seq)

if __name__ == "__main__":
    #testcase()
    #sys.exit()
    n = int(raw_input())
    for i in range(n):
        testcase(i+1)
