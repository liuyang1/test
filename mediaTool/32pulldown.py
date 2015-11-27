def ISR(infps, outfps):
    x = 0
    k = outfps / infps
    while True:
        yield int(x)
        x += k


def is32pulldown(pdt, dt):
    return (pdt == 3 and dt == 2) or (pdt == 2 and dt == 3)


def isDouble(pdt, dt):
    return dt == 2


if __name__ == "__main__":
    # when compare 23.97 60, use is32pulldown
    # when compare 29.97 60, use isDouble
    isr = ISR(29.97, 60)
    cur = 0
    dt = 0
    while 1:
        prev, cur = cur, isr.next()
        pdt, dt = dt, cur - prev
        if isDouble(pdt, dt):
            pass
        elif cur != 0 and cur != 2:
            print "break 3:2 pulldown at", cur, pdt, dt
            raw_input()
