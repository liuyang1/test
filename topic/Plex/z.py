import fw


wid = 4
ww = wid // 2
h0 = range(5 - ww, 5 + ww)
h1 = range(27 - ww, 27 + ww)


class zz(fw.Gen):

    def gen(self, x, y):
        if x < 5 - ww or x > 27 + ww:
            return 255
        if y < 5 - ww or y > 27 + ww:
            return 255
        if y in h0 or y in h1:
            return 0
        if x + y in range(32 - ww, 32 + ww + 1):
            return 0
        return 255

if __name__ == "__main__":
    w, h = 32, 32
    fmt = fw.AsciiGrayFmt
    fw.main(h, w, fmt, zz)
