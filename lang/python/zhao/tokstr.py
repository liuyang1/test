"""
My solution for [[http://blog.zhaojie.me/2009/10/code-for-fun-tokenizer.html]]

- token split with -
- group of token split with --
- shift with ' when have - in token
- double '' represent ' in token
"""


def rmQuote(s):
    if len(s) > 0 and s[0] == '\'' and s[-1] == '\'':
        s = s[1:-1]
    s = s.replace('\'\'', '\'')
    return s


def splitOneToken(s):
    shift = True
    for i in range(len(s)):
        if s[i] == '\'':
            shift = not shift
        elif shift and s[i] == '-':
            return s[:i], s[i + 1:]
    return s, None


def splitOneGroup(s):
    shift = True
    for i in range(len(s) - 1):
        if s[i] == '\'':
            shift = not shift
        elif shift and s[i] == '-' and s[i + 1] == '-':
            return s[:i], s[i + 2:]
    return s, None


def splitBy(s, fn):
    """functional style"""
    lst, t = [], s
    while t:
        h, t = fn(t)
        lst.append(h)
    return lst


def splitStr(s):
    g = splitBy(s, splitOneGroup)
    g = [splitBy(i, splitOneToken) for i in g]
    g = [[rmQuote(i) for i in j] for j in g]
    return g


if __name__ == "__main__":
    s = "cpu-3.0g--color-red-green-black--price-5000-8000--weight-'3-'--keywords-'levi''s'"
    lst = splitStr(s)
    print lst
