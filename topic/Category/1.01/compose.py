def compose(f, g):
    return lambda x: f(g(x))


def inc(x):
    return x + 1


def square(x):
    return x * x

x = 5
print (compose(inc, square))(x)
print (compose(square, inc))(x)

from id import *

print (compose(inc, id))(x)
print (compose(id, inc))(x)
print (compose(id, square))(x)
print (compose(square, id))(x)
