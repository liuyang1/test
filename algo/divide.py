"""
How to write a divide function without multiplcation, divide or modular?

equation:
a = b * x + y
x = a / b, y = a % b
"""


def divide(a, b):
    if a >= b:
        x, y = divide(a, b * 2)
    else:
        x, y = 0, a
    x *= 2
    if y >= b:
        x += 1
        y -= b
    print a, b, x, y
    return (x, y)


print divide(10, 4)
print divide(100, 1)
