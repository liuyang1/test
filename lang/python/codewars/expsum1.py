def genWu():
    n = 0
    nn = 0
    sign = -1
    while 1:
        nn += 2 * n + 1
        n += 1
        k = 3 * nn
        if sign == -1:
            sign = 1
        else:
            sign = -1
        yield sign, (k - n) // 2
        yield sign, (k + n) // 2

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
def exp_sum(n):
    if n < 0:
        return 0
    if n == 0 or n == 1:
        return 1
    g = genWu()
    lst = []
    while 1:
        sign, wu = next(g)
        lst.append((sign, wu))
        ni = n - wu
        if ni < 0:
            break
    v = 0
    for sign, wu in reversed(lst):
        ni = n - wu
        v += sign * exp_sum(ni)
    return v
