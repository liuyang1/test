from math import *
xs = [132464, 143640, 155424]
ns = [68, 70, 72]

lxs = map(log, xs)
lns = map(log, ns)


def diff(xs):
    return [a - b for a, b in zip(xs[1:], xs)]


def div(xs, ns):
    return [a / b for a, b in zip(xs, ns)]


ds = div(lxs, lns)
print ds
print min(ds)


print log(7) / log(2)
