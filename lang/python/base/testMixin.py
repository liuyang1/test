#! /usr/bin/env python

# Base Class don't have FUNC, but we could call it in base-class.
# This look like vitural function in OOP.

# Python is dynamic binding.

class BaseMixin(object):

    def callFunc(self):
        return self.func()


class InherMinin(BaseMixin):

    def func(self):
        print "test in func"


obj = InherMinin()
obj.callFunc()
