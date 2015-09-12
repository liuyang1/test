# python defaultdict style tree
# add generator style walk function
import collections


def Tree():
    return collections.defaultdict(Tree)


def walk(t, height=0):
    prefix = " " * 4 * height
    for k, v in t.iteritems():
        if type(v) == collections.defaultdict:
            print "%s%s" % (prefix, k)
            walk(v, height + 1)
        else:
            print "%s%s: %s" % (prefix, k, v)


def walkDfs(t, path=[]):
    for k, v in t.iteritems():
        pp = path + [k]
        if type(v) == collections.defaultdict:
            yield pp, None
            for i in walkDfs(v, pp):
                yield i
        else:
            yield pp, v


def test():
    t = Tree()
    # auto vivification feature
    t[1] = "Test"
    t[2][3] = "Test more"
    print t

    walk(t)

    for pp, v in walkDfs(t):
        print "%s: %s" % (pp, v)


if __name__ == "__main__":
    test()
