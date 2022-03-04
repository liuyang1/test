from matplotlib.pyplot import *
# from my import *
# plot(xs)

from my2 import *
xx = [x for (x, _) in xs]
yy = [y for (_, y) in xs]

# plot(xx, yy, "o")
loglog(xx, yy, "o")

savefig("2.png")
