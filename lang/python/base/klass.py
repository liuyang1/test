"""
class in python
every method is _virtual_
"""


class A():

    def f(self):
        print("class A f")

    def g(self):
        print("class A g")


class B(A):

    def f(self):
        print("class B bf")
        """
        directly call it at child class
        """
        self.g()

    def h(self):
        print("call super method g")
        print("this only python 3 style")
        super().g()

if __name__ == "__main__":
    print("super class")
    a = A()
    a.f()
    print("child class")
    b = B()
    print("override method f in A")
    b.f()
    print("virtual call method in super class")
    b.g()
    b.h()
