#! /usr/bin/env python3
"""
brute-force solution with sort
"""


def isGift2(a, b):
    return a != b and abs(a - b) <= 2


def isGift3(a, b, c):
    """
    >>> isGift3(1, 4, 7)
    False
    >>> isGift3(4, 3, 5)
    True
    >>> isGift3(30, 31, 32)
    True
    >>> isGift3(30, 31, 33)
    False
    """
    return isGift2(a, b) and isGift2(b, c) and isGift2(a, c)


def isGift(lst):
    """
    >>> isGift([18, 55, 16, 17])
    True
    >>> isGift([40, 41, 43, 44, 44, 44])
    False
    >>> isGift([5, 972, 3, 4, 1, 4, 970, 971])
    True
    """
    for i in range(len(lst)):
        for j in range(i + 1, len(lst)):
            for k in range(j + 1, len(lst)):
                if isGift3(lst[i], lst[j], lst[k]):
                    return True
    return False


def main():
    n = int(input())
    line = input()
    line = [int(i) for i in line.split()]
    line = line[0:n]
    r = "YES" if isGift(line) else "NO"
    print(r)


if __name__ == "__main__":
    main()
