# WRONG ANSWER on test 4
import sys
def shuru():
    s = input()
    n, k = [int(i) for i in s.split()]
    s = input()
    a = [int(i) for i in s.split()]
    return n, k, a


def prt(s):
    ss = ""
    for idx, i in sorted(s.items(), key=lambda x: x[0]):
        if i == 0:
            continue
        o = " ".join(["%d" % (idx + 1) for j in range(i)])
        if ss != "":
            ss += " " + o
        else:
            ss = o
    ss += "\n"
    return ss


def build(a, k):
    a0 = min(a[0], k)
    s = {}
    for i in range(k):
        if i < a0:
            s[i] = 1
        else:
            s[i] = 0
    o = prt(s)
    for i in range(len(a) - 1):
        d = abs(a[i + 1] - a[i])
        if a[i + 1] > a[i]:
            sign = 1
            rev = False
        else:
            sign = -1
            rev = True
        cnt = 0
        for idx, i in sorted(s.items(), key=lambda x: x[1], reverse=rev):
            if i == 0 and sign < 0:
                continue
            if cnt == d:
                break
            i = i + sign
            s[idx] = i
            cnt += 1
        if cnt != d:
            print("NO")
            return
        o += prt(s)
    print("YES")
    print(o, end="")

if __name__ == "__main__":
    n, k, a = shuru()
    build(a, k)
