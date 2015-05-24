def memoize(f):
    """ Memoization decorator for functions taking one or more arguments. """
    class memodict(dict):
        def __init__(self, f):
            self.f = f
        def __call__(self, *args):
            return self[args]
        def __missing__(self, key):
            ret = self[key] = self.f(*key)
            return ret
    return memodict(f)

@memoize
def fn(n, m):
    if n == 1 or m == 1:
        return 1
    elif n < m:
        return fn(n, n)
    elif n == m:
        return 1 + fn(n, n - 1)
    else:
        return fn(n, m - 1) + fn(n - m, m)

def exp_sum(x):
    """
    >>> exp_sum(0)
    1
    >>> exp_sum(-1)
    0
    >>> exp_sum(5)
    7
    >>> exp_sum(10)
    42
    """
    if x < 0:
        return 0
    elif x == 0:
        return 1
    return fn(x, x)
