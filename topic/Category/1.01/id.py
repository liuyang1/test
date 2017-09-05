def id(x):
    return x

print id(1)
print id([1, 2, 3])
print id((1, 2, 3))
print id({k: k + 1 for k in range(3)})
print id("Hello, identify")
