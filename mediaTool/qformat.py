from math import *

q=12
# xs: dB, -32dB to 32dB
xs = [i - 32 for i in range(65)]
#xs = [-6]
# ys: factor with q value
ys = [int(round(10**(i/20.)*(2**q))) for i in xs]
# zs: reversed exact dB value
zs = [20*log(i/(2.0**q), 10) for i in ys]
print(ys)
print(zs)
