import random
import sys

def simu(num, N=10000,times = 10):
    def f():
        ret = []
        for i in xrange(num):
            v = random.randint(0, N)
            ret.append(v)
        from collections import Counter
        c = Counter(ret)
        return len(c.keys())
    v = 0
    for i in xrange(times):
        v += f()
    return v/times

for i in xrange(1000, 1000*20+1, 1000):
    print i, simu(i)
    sys.stdout.flush()
