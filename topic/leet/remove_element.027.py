def remove_element(xs, e):
    ys = []
    for i, x in enumerate(xs):
        if x != e:
            ys.append(x)
    return ys


def unit(xs, x, expect):
    r = remove_element(xs, x)
    if r != expect:
        print("fail when remove_element(%s, %d) => %s != %s" % (xs, x, r, expect))


unit([0,1,2,2,3,0,4,2],2, [0,1,3,0,4])
