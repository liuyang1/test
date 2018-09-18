# NOT FINISH
def buildn(s):
    """
    >>> buildn(4)
    4
    >>> buildn(10)
    19
    >>> buildn(20)
    299
    >>> buildn(21)
    399
    """
    n, d = 0, 0
    while s > 9:
        n = 10 * n + 9
        s -= 9
        d += 1
    if s != 0:
        if n != 0:
            n = s * (10 ** d) + n
        else:
            n = s
    return n


def buildnm(s, m):
    """
    >>> buildnm(4, 4)
    40
    """
    return 4
