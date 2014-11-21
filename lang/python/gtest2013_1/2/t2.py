#! /usr/bin/env python

def calcN(num):
    if num==1:
        return (1,1)
    p,q = calcN(num/2)
    rem = num%2
    if rem == 0:
        q = p + q
    else:
        p = p + q
    return (p,q)

def calcPos(p,q):
    if p<q:
        q = q - p
        return calcPos(p,q)*2
    elif p>q:
        p = p - q
        return calcPos(p,q)*2 + 1
    else:
        return 1

def testcase(seq):
    line = raw_input()
    line = line.split()
    line = [int(i) for i in line]
    if line[0]==1:
        ret = calcN(line[1])
        print "Case #%d: %d %d"%(seq,ret[0],ret[1])
    elif line[0]==2:
        ret = calcPos(line[1],line[2])
        print "Case #%d: %d"%(seq,ret)

if __name__ == "__main__":
    n = int(raw_input())
    for i in range(n):
        testcase(i+1)
