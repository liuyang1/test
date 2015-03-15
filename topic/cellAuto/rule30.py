#! /usr/bin/env python
"""
ref: http://www.guokr.com/article/439770/
"""
import sys


def bitarray(v):
    """
    >>> bitarray(30)
    [0, 1, 1, 1, 1]
    """
    if v == 0:
        return [0]
    lst = []
    while v != 0:
        lst.append(v % 2)
        v //= 2
    return lst


def crtRule(v):
    tbl = bitarray(v)
    def func(n):
        n_1, n0, n1 = n
        v = n_1 * 4 + n0 * 2 + n1
        if v >= len(tbl) or tbl[v] == 0:
            return 0
        else:
            return 1
    return func


def rule30(n):
    """
    3 -> 2 ^ 3 = 8 bit -> 0b00011110 -> 30
    """
    n_1, n0, n1 = n
    v = n_1 * 4 + n0 * 2 + n1
    return 1 if 1 <= v <= 4 else 0


def iterRule(fn, lst):
    lst = [0, 0] + lst + [0, 0]
    com = zip(lst, lst[1:], lst[2:])
    lst = map(fn, com)
    return list(lst)


def show(lst):
    s = ["*" if x == 1 else " " for x in lst]
    return "".join(s)


if __name__ == "__main__":
    itern = 100
    for r in range(129):
        rule = crtRule(r)
        print(r)
        fp = open("%03d.ret" % (r), "w")
        lst = [1]
        for i in range(itern):
            s = " " * (itern - i) + show(lst)
            fp.write(s + "\n")
            lst = iterRule(rule, lst)
        fp.close()
