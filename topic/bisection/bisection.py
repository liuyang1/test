from matplotlib import cm
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import numpy as np
from math import *

@np.vectorize
def ratio(n, p):
    ps = [(1-p)**n, n*p*(1-p)**(n-1)]
    ps.extend([1-sum(ps)])
    # print(ps)
    tt = [1, 2*log(n)+2, n+4]
    # print(tt)
    r = sum([p*t for (p, t) in zip(ps, tt)])
    # print(r)
    return r / n

nn = np.linspace(2, 64, 100)
pp = np.logspace(-2, -0.3, 100)

N, P = np.meshgrid(nn, pp)
print(N, P)

R = ratio(N, P)
print(R)

fig = plt.figure()
# ax = fig.add_subplot(projection='3d')
# ax.set_xscale('log')
# surf = ax.plot_surface(N, P, R, cmap=cm.coolwarm,
#                        linewidth=0, antialiased=True)
# fig.colorbar(surf, shrink=0.5, aspect=5)
plt.contourf(N, P, R, levels=np.logspace(-2, 0, 20))
plt.xscale('log')  # 设置 X 轴为对数刻度
plt.yscale('log')  # 设置 Y 轴为对数刻度
plt.xlabel('bulk size/N')
plt.ylabel('bad CR prob/%')
plt.colorbar()
plt.show()
