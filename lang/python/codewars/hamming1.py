def isfactorl(n, lst):
    """
    >>> isfactorl(10, [2,5])
    True
    """
    def factor(n, f):
        if n == 0:
            return 0
        while n % f == 0:
            n //= f
        return n
    for f in lst:
        n = factor(n, f)
    return n == 1

def hamming(n):
    """
    >>> hamming(1)
    1
    >>> hamming(10)
    12
    >>> hamming(100)
    1536
    """
    if n == 1:
        return 1
    idx = 1
    i = 1
    while 1:
        i += 1
        if isfactorl(i, [2, 3, 5]):
            idx += 1
        if idx == n:
            return i
