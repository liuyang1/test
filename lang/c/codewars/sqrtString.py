def sqrt(n):
    def bound():
        i = 1
        while i * i < n:
            i *= 2
        return i
    def shrink(a, b):
        m = (a + b) / 2
        m2 = m * m
        if m2 == n:
            return m, m
        elif m * m > n:
            return a, m
        else:
            return m, b
    b, e = 0, bound()
    step = 1
    while True:
        step += 1
        b, e = shrink(b, e)
        if e - b <= 1:
            print "step: ", step
            return b


print sqrt(101)
print sqrt(23232328323215435345345345343458098856756556809400840980980980980809092343243243243243098799634)
