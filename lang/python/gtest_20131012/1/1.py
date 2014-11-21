def trans(mat):
    return map(list, zip(*mat))


def isSudoku(mat, n):
    s = set([i for i in range(1, n * n + 1)])
    for r in mat:
        if set(r) != s:
            return False
    for l in trans(mat):
        if set(l) != s:
            return False
    for i in range(0, n * n, n):
        for j in range(0, n * n, n):
            data = mat[i:i+3]
            data = [k[j:j+3] for k in data]
            all = []
            for d in data:
                all.extend(d)
            if set(all) != s:
                return False
    return True



def testcase(seq):
    n = int(raw_input())
    sudoku = []
    for i in range(n * n):
        l = raw_input()
        l = [int(i) for i in l.split()]
        sudoku.append(l)
    ret = isSudoku(sudoku, n)
    if ret:
        print "Case #%d: Yes" % (seq)
    else:
        print "Case #%d: No" % (seq)


if __name__ == "__main__":
    n = int(raw_input())
    for i in range(n):
        testcase(i + 1)
