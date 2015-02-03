#! /usr/bin/env python3


AsciiBinFmt, AsciiGrayFmt, AsciiPixelFmt = range(1, 4)
BinBinFmt, BinGrayFmt, BinPixelFmt = range(4, 7)

ext = ["", "pbm", "pgm", "ppm", "pbm", "pgm", "ppm"]

def isBinFmt(fmt):
    return fmt % 3 == 1
def isGrayFmt(fmt):
    return fmt % 3 == 2
def isPixelFmt(fmt):
    return fmt % 3 == 0


class Gen():

    def __init__(self, width, height, fmt, maxval=255):
        self.width = width
        self.height = height
        self.fmt = fmt
        self.maxval = 1 if isBinFmt(fmt) else maxval

    def gen(self, x, y):
        wid = 1
        xx, yy = x // wid, y // wid
        v = (xx + yy) % 2
        # print (x, y, xx, yy, v)
        if v == 1:
            v = self.maxval
        return '%3d' % (v)

def PPMHeader(width, height, fmt=AsciiPixelFmt, maxval=255):
    s = "P%d\n" % (fmt)
    s += "%d %d\n" % (width, height)
    if not isBinFmt(fmt):
        s += "%d\n" % (maxval)
    return s


def main(w, h, fmt, GEN):
    g = GEN(w, h, fmt)
    with open("test." + ext[fmt], "w") as fp:
        print("width: %d height: %d fmt: %d" % (w, h, fmt))
        fp.write(PPMHeader(w, h, fmt))
        for i in range(h):
            for j in range(w):
                fp.write(" %s" % (g.gen(j, i)))
            fp.write("\n")


if __name__ == "__main__":
    v = 64
    w, h = v, v
    fmt = AsciiBinFmt
    main(w, h, fmt, Gen)
