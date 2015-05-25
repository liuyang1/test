def hammingg():
    lst = [1]
    while 1:
        ret = lst[0]
        newlst = [ret * i for i in [2, 3, 5]]
        lst = lst[1:] + newlst
        lst = set(lst)
        lst = sorted(list(lst))
        yield ret


def hamming(n):
    h = hammingg()
    idx = 1
    while idx < n:
        v = next(h)
        idx += 1
    return v


print(hamming(2000))
