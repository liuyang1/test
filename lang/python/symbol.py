import time
syms = ['-', '\\', '|', '/']

c = 0
while 1:
    for i in syms:
        print(i + "\b", end="")
        time.sleep(0.1)
    c += 1
