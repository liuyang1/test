def func():
    raise Exception('spam', 'eggs')

try:
    func()
except Exception as e:
    print type(e)
    print e
    print e.args
