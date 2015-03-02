import sys
def queen(row, col):
    def safe(l, k):
        for idx, i in enumerate(l):
            if i == k or i + idx + 1 == k or i - idx - 1 == k:
                return False
        return True

    if col == 0:
        return [[]]
    lst = queen(row, col - 1)
    return [[k] + l for k in range(row) for l in lst if safe(l, k)]


def disp(lst):
    m = max(lst)
    for i in lst:
        for j in range(m + 1):
            ec = " " if j != m else "\n"
            cc = "-" if j != i else "*"
            print(cc, end=ec)


def isSyms(ans0, ans1):
    row = len(ans0)
    col = max(ans0) + 1
    # 左右对称
    if all(ans0[i] + ans1[i] == col for i in range(row)):
        return True
    # 上下对称
    if all(ans0[i] == ans1[row - 1 - i] for i in range(row)):
        return True
    # 中心对称
    if all(ans0[i] + ans1[row - 1 - i] == col for i in range(row)):
        return True
    # 相同
    if all(ans0[i] == ans1[i] for i in range(row)):
        return True
    if row == col:
        # 45 对称
        if all(ans1[ans0[i]] == i for i in range(row)):
            return True
        if all(ans1[ans0[i]] == row - 1 - i for i in range(row)):
            return True
        if all(ans1[row - 1 - ans0[i]] == i for i in range(row)):
            return True
        # 135 对称
        if all(ans1[row - 1 - ans0[i]] == row - 1 - i for i in range(row)):
            return True
        # 旋转
    return False


def filterSyms(lst):
    def isSymslst(ans, anslst):
        return any(isSyms(ans, a) for a in anslst)
    alst = [lst[0]]
    for a in lst[1:]:
        if not isSymslst(a, alst):
            alst.append(a)
    return alst

ans = queen(10, 10)
print(ans)
print(len(ans))
if len(ans) == 0:
    sys.exit()
disp(ans[0])
print()

ans = filterSyms(ans)
print(ans)
print(len(ans))
# sys.exit()
for i in ans:
    disp(i)
    print()
