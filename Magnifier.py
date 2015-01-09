import fw


class Magnifier(fw.Gen):
    def gen(self, x, y):
        wid = 2
        xx, yy = x // wid, y // wid
        v = (xx + yy) % 3
        print (xx, yy, v)
        if v == 0:
            return 0
        elif v == 1:
            return 128
        else:
            return self.maxval


if __name__ == "__main__":
    w, h = 1024, 1024
    fmt = fw.AsciiPixelFmt
    fw.main(h,w, fmt, Magnifier)
