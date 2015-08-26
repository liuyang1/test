import contextlib

@contextlib.contextmanager
def htmlTag(tag):
    print "<%s>" % (tag)
    yield
    print "</%s>" % (tag)


with htmlTag("h1"):
    print "Header"
