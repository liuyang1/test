N = 100
x = [(1. / N) * i for i in range(N + 1)]

white = x
blue = [i ** 2 for i in x]
brown = [2 * i - i ** 2 for i in x]

from pylab import *
plot(x, white, color="gray")
plot(x, blue, color="blue")
plot(x, brown, color="brown")

show()
