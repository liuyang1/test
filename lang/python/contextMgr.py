class myCtxMgr():
    def __init__(self, val):
        print "init"
        self.val = val
    def __enter__(self):
        print "enter"
        return self.val + 1, self.val
    def __exit__(self, type, value, traceback):
        print "leave"
        return

with myCtxMgr(5) as (val, val2):
    print val
