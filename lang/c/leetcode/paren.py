def catalanParen(s, l, r):
    if l == 0 and r == 0:
        print s
    if l != 0:
        catalanParen(s + "(", l - 1, r)
    if r > l:
        catalanParen(s + ")", l, r - 1)


def paren(n):
    catalanParen("", n, n)


if __name__ == "__main__":
    paren(5)
