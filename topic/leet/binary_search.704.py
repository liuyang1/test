def search(xs, x, left, right):
    while left < right:
        mid = (left + right) / 2;
        if xs[mid] == x:
            return mid
        elif xs[mid] > x:
            right = mid
        else:
            left = mid + 1
    return -1


xs = [1, 2, 3, 4, 7, 9, 10]

def unit(xs, x, e):
    r = search(xs, x, 0, len(xs))
    if r != e:
        print("fail when search %d in %s, return %d, expect %d", x, xs, r, e)


for i, x in enumerate(xs):
    unit(xs, x, i)

unit(xs, 0, -1)
unit(xs, 11, -1)
