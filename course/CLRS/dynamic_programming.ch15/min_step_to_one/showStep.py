#! /usr/bin/env python2

from pylab import *
import sys
fn = "step.txt"


def loadData(f):
    with open(f) as fp:
        lst = []
        lines = fp.read().split('\n')
        for line in lines:
            if len(line) == 0 or line[0] == '#':
                break
            xx = line.split()
            v = [int(i) for i in xx]
            lst.append(v)
        return lst


data = loadData(fn)
# print data
data = data[0]

# style.use('ggplot')
_, ax = subplots()
grid()
# ax.plot(data, ".")
ax.semilogx(data, ".")

xlabel("N")
ylabel("step")


savefig(fn + ".png")
