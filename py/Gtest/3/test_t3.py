#! /usr/bin/env python
from t3 import *


def test_count():
    assert 1 == count([1, 3, 2])
    assert 0 == count([1, 2, 3])
