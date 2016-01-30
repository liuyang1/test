
def fab(max):
    n, a, b = 0, 0, 1
    while n<max:
        yield b
        a, b = b, a + b
        n = n + 1

print "first"
for n in fab(10):
    print n

print "second"
f = fab(100)
for n in range(10):
    print n, f.next()

print "third"
f0 = fab(5)
f1 = fab(10)
f = "f0"
while True:
    f = "f1" if f == "f0" else "f0"
    try:
        n = next(eval(f))
    except:
        break
    print n
