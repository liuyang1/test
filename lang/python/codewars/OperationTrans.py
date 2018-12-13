def operation(a, b):
    a, b = b, a
    step = 0
    st = {a}
    while b not in st:
        step += 1
        nst = set()
        for i in st:
            # nst |= {(i - 1) / 2}
            nst |= {2 * i + 1}
            nst |= {2 * i}
            nst |= {i / 2}
        st = nst
    print(st)
    for i in st:
        print(i, end=",")
        print(bin(int(i)), end=", ")
        if i != int(i):
            print(bin(int(1 / (i - int(i)))), end="")
        print("")
    return step


print(operation(1, 1))
print("------")
print(operation(1, 4))
print("------")
print(operation(4, 1))
print("------")
# print(operation(1, 1024 * 1024))
print(operation(128, 1024))
