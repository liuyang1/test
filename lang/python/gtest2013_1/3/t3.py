#! /usr/bin/env python
def calc(seq):
    label = [i%2 for i in seq]
    jishu = [ i for i in seq if i%2==1]
    oushu = [ i for i in seq if i%2==0]
    jishu = sorted(jishu)
    oushu = sorted(oushu,reverse = True)
    jiidx = 0
    ouidx = 0
    for i in range(len(seq)):
        li = label[i]
        if li==1:
            seq[i]=jishu[jiidx]
            jiidx += 1
        else:
            seq[i]=oushu[ouidx]
            ouidx += 1
    return seq

def printSeq(seq):
    for i in seq:
        print i,

def testcase(seq):
    n = int(raw_input())
    line = raw_input()
    line = line.split()
    line = [int(i) for i in line]
    ret = calc(line)
    print "Case #%d:"%(seq),
    printSeq(ret)
    print 

if __name__ == "__main__":
    n = int(raw_input())
    for i in range(n):
        testcase(i+1)
