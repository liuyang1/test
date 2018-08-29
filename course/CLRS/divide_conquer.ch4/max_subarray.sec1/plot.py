from pylab import *


def loadData(f):
    with open(f) as fp:
        lst = []
        lines = fp.read().split('\n')
        head = lines[0].split()
        for line in lines[1:]:
            v = [int(i) for i in line.split()]
            if len(v) != 0:
                lst.append(v)
        return head, zip(*lst)


head, data = loadData("perf.txt")
x = data[0]
ys = data[1:]

# style.use('ggplot')
_, ax = subplots()
grid()
for y in ys:
    ax.loglog(x, y)

xlabel(head[0])
ylabel("time/us")
legend(head[1:], loc='upper left')
title("Algo time complexity")

savefig('perf.png')
