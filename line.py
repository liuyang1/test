import fw

a, b, c = 1, -1, 0
a1, b1, c1 = 1, -2, -100
class Line(fw.Gen):
    def gen(self, x, y):
        if a * x + b * y + c == 0:
            return 0
        if a1 * x + b1 * y + c1 == 0:
            return 0
        return 255


if __name__ == "__main__":
    w, h = 512, 512
    fmt = fw.AsciiGrayFmt
    fw.main(h, w, fmt, Line)

