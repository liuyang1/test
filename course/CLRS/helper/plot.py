#! /usr/bin/env python2

from pylab import *
import sys
fn = sys.argv[1]


def loadData(f):
    with open(f) as fp:
        lst = []
        lines = fp.read().split('\n')
        for line in lines:
            if len(line) == 0 or line[0] == '#':
                break
            xx = line.split()
            head = xx[0]
            v = [int(i) for i in xx[1:]]
            lst.append((head, v))
        return lst


data = loadData(fn)
print data

# style.use('ggplot')
_, ax = subplots()
grid()
for x, y in zip(data[::2], data[1::2]):
    ax.plot(x[1], y[1], '-')


savefig(fn + ".png")
