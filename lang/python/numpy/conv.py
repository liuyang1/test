import numpy as np

lst = np.array([1 for i in range(99)], dtype="object")


def pow(lst, n):
    if n == 1:
        return lst
    elif n % 2 == 0:
        l = pow(lst, n / 2)
        return np.convolve(l, l)
    else:
        return np.convolve(lst, pow(lst, n - 1))


ans = pow(lst, 4)
print ans
print np.cumsum(ans)
