class myCtxMgr():
    def __init__(self, val):
        print "init"
        self.val = val
    def __enter__(self):
        print "enter"
        return self.val
    # Don't use keyword type as var name
    # as Guan XiQing's suggestion
    def __exit__(self, typ, value, traceback):
        print "leave"
        return

with myCtxMgr(5) as val:
    print val
