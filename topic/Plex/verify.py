#! /usr/bin/env python3
with open("test.pgm") as fp:
    lineno = 0
    for line in fp.readlines():
        lineno+= 1
        if lineno == 2:
            break
    lineno = 0
    for line in fp.readlines():
        lst = [int(v) for v in line.split()]
        for i in range(len(lst)):
            if (i + lineno) % 2 == 0:
                if lst[i] == 0:
                    continue
            else:
                if lst[i] == 255:
                    continue
            print (lineno, i, "error", lst[i])
        lineno += 1
