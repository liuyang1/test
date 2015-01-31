#! /usr/bin/env python3
"""
regular polygon

get Circumcircle of trianglur, then analysis on the angle,
will get answer.
"""
from functools import reduce

# runtime error on test 47
# 2015-01-10, update but still this error
import math


def det3(mat):
    v = mat[0][0] * mat[1][1] * mat[2][2]
    v += mat[0][1] * mat[1][2] * mat[2][0]
    v += mat[0][2] * mat[1][0] * mat[2][1]
    v -= mat[0][2] * mat[1][1] * mat[2][0]
    v -= mat[0][1] * mat[1][0] * mat[2][2]
    v -= mat[0][0] * mat[1][2] * mat[2][1]
    return v


square = lambda x: x * x


def squareSum(pt):
    return square(pt[0]) + square(pt[1])


def CircumCentre(ptA, ptB, ptC):
    """
    >>> CircumCentre((0, 0), (1, 1), (0, 1))
    (0.5, 0.5)
    """
    den = 2. * \
        det3([[ptA[0], ptA[1], 1], [ptB[0], ptB[1], 1], [ptC[0], ptC[1], 1]])
    aa = squareSum(ptA)
    bb = squareSum(ptB)
    cc = squareSum(ptC)
    xx = det3([[aa, ptA[1], 1], [bb, ptB[1], 1], [cc, ptC[1], 1]])
    yy = det3([[ptA[0], aa, 1], [ptB[0], bb, 1], [ptC[0], cc, 1]])
    xx /= den
    yy /= den
    return (xx, yy)


def distance2(ptA, ptB):
    R2 = squareSum([ptA[0] - ptB[0], ptA[1] - ptB[1]])
    return R2


def distance(ptA, ptB):
    return math.sqrt(distance2(ptA, ptB))


def deCos(a2, b2, c2):
    return (b2 + c2 - a2) / 2 * b2 * c2


def deCircumCos(l2, r2):
    return math.acos(1. - l2 / (2. * r2))


def guiyi(theta):
    return theta / 2. / math.pi


def CircumAngle(ptA, ptB, ptC, pt):
    """
    >>> CircumAngle((0, 0), (1, 1), (0, 1), (0.5, 0.5))
    (0.25, 0.25, 0.5)
    """
    # pt = CircumCentre(ptA, ptB, ptC)
    R2 = squareSum([ptA[0] - pt[0], ptA[1] - pt[1]])
    a = deCircumCos(distance2(ptB, ptC), R2)
    b = deCircumCos(distance2(ptC, ptA), R2)
    c = deCircumCos(distance2(ptA, ptB), R2)
    return guiyi(a), guiyi(b), guiyi(c)


def toFactor(n):
    """
    >>> toFactor(0.5)
    (1, 2)
    >>> toFactor(0.3333)
    (1, 3)
    >>> toFactor(2/7.)
    (2, 7)
    """
    eps = 0.01
    for den in range(2, 101):
        num = den * n
        d = num - round(num)
        if abs(d) < eps:
            return (int(round(num)), den)
    return (int(round(100 * n)), 100)


def gcd(a, b):
    """
    >>> gcd(6, 15)
    3
    """
    if b == 0:
        return a
    elif a < b:
        return gcd(b, a)
    else:
        return gcd(b, a % b)


def lcm(a, b):
    return a * b / gcd(a, b)


def MaxEdgeN(l):
    """
    >>> MaxEdgeN([6, 15, 10])
    30
    """
    return reduce(lcm, l, 1)


def EdgeNum(ptA, ptB, ptC, pt):
    """
    >>> EdgeNum((0, 0), (1, 1), (0, 1), (0.5, 0.5))
    4
    """
    a, b, c = CircumAngle(ptA, ptB, ptC, pt)
    edg = [toFactor(i) for i in [a, b, c]]
    edgn = MaxEdgeN([d for n, d in edg])
    return edgn


def Area(R2, N):
    """
    >>> Area(0.5, 4)
    1.0
    """
    N = (N + 0.)
    theta = math.pi / N
    return R2 * N * math.sin(theta) * math.cos(theta)


def main():
    lst = []
    for i in range(3):
        line = raw_input()
        pt = [float(i) for i in line.split()]
        lst.append(pt)
    ptA = lst[0]
    pt = CircumCentre(lst[0], lst[1], lst[2])
    N = EdgeNum(lst[0], lst[1], lst[2], pt)
    R2 = squareSum([ptA[0] - pt[0], ptA[1] - pt[1]])
    print("%.8f" % Area(R2, N))


if __name__ == "__main__":
    main()
