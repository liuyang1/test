#!/usr/bin/env python2
#-*- encoding:utf8
"""
启发自Thinking Functionaly with Haskell一书，P36的floor函数实现方法
"""


def sqrt(n):
    def invFunc(i):
        return i * i

    def bound():
        i = 1
        while invFunc(i) <= n: # must add equal
            i *= 2
        return i

    def shrink(a, b):
        m = (a + b) / 2
        m2 = invFunc(m)
        if m2 == n:
            return m, m
        elif m2 > n:
            return a, m
        else:
            return m, b

    def unit(b, e):
        return e - b <= 1

    b, e = 0, bound()
    while True:
        b, e = shrink(b, e)
        if unit(b, e):
            break
    return b


for i in range(200):
    sqr = sqrt(i)
    if not sqr * sqr <= i < (sqr + 1) * (sqr + 1):
        print "find except on %d, %d" % (i, sqr)
        break
print sqrt(23232328323215435345345345343458098856756556809400840980980980980809092343243243243243098799634)
