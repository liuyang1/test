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
