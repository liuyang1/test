import sys


def wrFile(fn, content):
    print "write %s" % (fn)
    with open(fn, "w") as fp:
        fp.write(content)


def main():
    seg = 100000
    fn = sys.argv[1]
    with open(fn) as fp:
        linecnt = 0
        s = ""
# DONOT use read or readlines, they load entire file into memory
        for line in fp:
            s += line
            linecnt += 1
            if linecnt % seg == (seg - 1):
                newfn = "%s.%03d" % (fn, linecnt / seg)
                wrFile(newfn, s)
                s = ""
        if linecnt < seg:
            print "less than seg, not need split"
            return
        if linecnt % seg != (seg - 1):
            newfn = "%s.%03d" % (fn, linecnt / seg)
            wrFile(newfn, s)

if __name__ == "__main__":
    main()
