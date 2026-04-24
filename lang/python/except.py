def func():
    raise Exception('spam', 'eggs')

try:
    func()
except Exception as e:
    print(type(e))
    print(e)
    print(e.args)

import sys
print(sys.argv)
if len(sys.argv) == 1 or sys.argv[1] != "verbose":
    assert True
    assert False
else:
    assert True, "msg"
    assert False, "msg"
