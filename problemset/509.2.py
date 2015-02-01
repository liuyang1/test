import copy
import sys
def inputData():
    s = input()
    n, k = [int(i) for i in s.split()]
    s = input()
    a = [int(i) for i in s.split()]
    return n, k, a


def isOK(a, k):
    return max(a) - min(a) <= k

def build(a, k):
    a0 = min(a)
    d = {}
    d[a0] = [(0, a0)]
    lst = [(0, a0 + 1)]
    d[a0 + 1] = lst
    color = 1
    for i in range(a0 + 2, max(a) + 1):
        lst = copy.deepcopy(lst)
        lst.append((color, 1))
        color += 1
        d[i] = lst
    for i in a:
        l = d[i]
        for c, n in l:
            for i in range(n):
                print(c + 1, end=" ")
        print()

if __name__ == "__main__":
    n, k, a = inputData()
    if not isOK(a, k):
        print("NO")
    else:
        print("YES")
        build(a, k)
