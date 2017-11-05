def divide(x, y):
    y2 = y * 2
    if x > y2:
        a, b = divide(x, y2)
        a *= 2
    else:
        a, b = 0, x
    if b > y:
        a, b = a + 1, b - y
    print "%d / %d = %d, %d" % (x, y, a, b)
    return a, b


def unit(x, y):
    a, b = divide(x, y)
    if a * y + b == x and b < y:
        return True
    else:
        print "fail at %d / %d = %d, %d" % (x, y, a, b)

unit(12, 5)
unit(123847897, 13245987)
unit(1, 13245987)
unit(1232398475, 13)

