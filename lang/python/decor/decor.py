
def wrap(foo):
	def wrap1(func):
		def wraparg(*args,**kwargs):
			print foo
			func(*args,**kwargs)
			print "decor over"
		wraparg.func_name=func.func_name
		wraparg.__doc__=func.__doc__
		return wraparg
	return wrap1

num=12
@wrap(num)
def testfunc(s):
	print "testfunc"
	print s
	print "testfunc over"

testfunc("hello,decor")
