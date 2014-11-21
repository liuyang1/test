def enhanced(meth):
	def new(self,y):
		print "enhancde"
		return meth(self,y)
	return new

class Foo:
	@classmethod
	def foo(cls,x):
		print "classmethod",cls,x
	@staticmethod
	def bar(y):
		print "staticmethod",y
	@enhanced
	def baz(self,z):
		print self,"baz say:",z

Foo.foo(100)
d=Foo()
d.foo(200)

Foo.bar(300)
d.bar(400)

Foo.baz(d,500)
d.baz(600)
