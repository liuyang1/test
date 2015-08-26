import contextlib

@contextlib.contextmanager
def myCtxMgr(val):
    print "init"
    print "enter"
    yield val
    print "leave"

with myCtxMgr(5) as val:
    print val
