from heapq import *


def parent(n):
    return (n - 1) / 2


def left(n):
    return 2 * n + 1


def right(n):
    return 2 * n + 2


def myheappush(xs, y):
    xs = xs.append(y)
    n = len(xs)
    while n != 0:
        if xs >


def insert(xs, y):
    beg, end = 0, len(xs)
    while beg < end:
        mid = (beg + end) / 2
        if y == xs[mid]:
            return xs
        else if y < xs[mid]:
            end = mid - 1
        else
            beg = mid + 1
    print xs, beg, mid
    return xs

def dbl_linear(n):
    h = []
    heappush(h, 1)
    for i in range(n):
        v = heappop(h)
        heappush(h, 2 * v + 1)
        heappush(h, 3 * v + 1)
    return v

def test():
    print dbl_linear(1)
    print dbl_linear(2)
    print dbl_linear(3)
    print dbl_linear(4)
    print dbl_linear(1000)
