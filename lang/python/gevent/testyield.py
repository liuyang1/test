
def fab(max):
    n, a, b = 0, 0, 1
    while n<max:
        yield b
        a, b = b, a + b
        n = n + 1

for n in fab(10):
    print n

f = fab(100)
for n in range(10):
    print n, f.next()
