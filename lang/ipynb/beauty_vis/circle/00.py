from math import *
from matplotlib.pylab import *


def paramCircle(x0, y0, theta=0, r=1, **kw):
    """
    参数方程，经过点x，y，角度为theta，半径为r的圆
    其圆心则为x0+r*cos(theta),y0+r*sin(theta)
    半径为r
    """
    cx = x0 + r * cos(theta)
    cy = y0 + r * sin(theta)
    c = Circle((cx, cy), r, **kw)
    ax.add_patch(c)


def genStyle():
    bk, fg = "#424434", "#fac34f"
    for i in range(1000):
        yield {"facecolor": bk} if i % 2 == 0 else {"facecolor": fg}
        # yield {"fill": None}


def paramCircles(x0, y0, theta, rs, gsty):
    for i, r in enumerate(rs):
        paramCircle(x0, y0, theta, r, **next(gsty))


def Plot():
    gsty = genStyle()
    # 画出背景的圆
    r = 1
    paramCircle(-1, 0, 0, 1, **next(gsty))  # facecolor=fg)
    # 画出N一系列的，经过cos(alpha), sin(alpha), 方向为alpha+pi,半径为rs的圆
    N = 13
    rate = 0.5 ** (1.0 / N)
    rs = [rate**i for i in range(1, N)]
    alpha = pi / 4 + pi
    paramCircles(1 * cos(alpha), 1 * sin(alpha), alpha + pi, rs, gsty)
    # 画出N系列的，经过0，0，方向为alpha，半径为rs的圆
    rs = [rate**i for i in range(N, 2 * N)]
    paramCircles(0, 0, alpha, rs, gsty)
    # 换出N系列的，经过。。，方向为beta+pi,半径为rs的圆
    beta = pi / 4 # WARNING: same orientation with step 2
    r0 = rs[-1]
    rs = [rate**i * r0 for i in range(1, 2 * N)]
    paramCircles(
        r0 * cos(alpha) + r0 * cos(beta),
        r0 * sin(alpha) + r0 * sin(beta),
        beta + pi,
        rs,
        gsty,
    )


def seqCircle(xs, ys, rs, **kw):
    for x, y, r in zip(xs, ys, rs):
        # circle1 = Circle((x, y), r, fill=None)# alpha=0.1, **kw)
        circle1 = Circle((x, y), r, alpha=0.05, **kw)
        print(x, y, r)
        ax.add_patch(circle1)


def seqPlot():
    N = 13
    rate = 0.5 ** (1.0 / (N - 1))
    rs = [rate**i for i in range(N)]
    xs = [r - 1 for r in rs]
    seqCircle(xs, [0] * N, rs)

    rs = [rate**i for i in range(N, N * 2 - 1)]
    xs = [-r for r in rs]
    seqCircle(xs, [0] * N, rs, color="green")

    rs = [rate**i for i in range(N * 2 - 1, N * 3 - 2)]
    xs = [r - 0.5 for r in rs]
    seqCircle(xs, [0] * N, rs, color="yellow")

    rs = [rate**i for i in range(N * 3 - 2, N * 4 - 3)]
    xs = [-0.25 - r for r in rs]
    seqCircle(xs, [0] * N, rs, color="orange")


figure(figsize=(8, 8))
ax = subplot(aspect="equal")
xlim(-1, 1)
ylim(-1, 1)
Plot()
axis("off")
show()
savefig("00.png")
