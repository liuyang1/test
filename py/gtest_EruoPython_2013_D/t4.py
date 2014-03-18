#! /usr/bin/env python


def printMat(mat):
    for r in mat:
        for s in r:
            print "%2d" % (s),
        print
    return


def getMatVal(mat, x, y):
    row = len(mat)
    col = len(mat[0])
    if x < 0 or x >= row or y < 0 or y >= col:
        return False
    else:
        return mat[x][y]


def addMatVal(mat, x, y):
    mat[x][y] = 0 if mat[x][y] 


def getNegb(mat, x, y):
    vv = (getMatVal(mat, x - 1, y), getMatVal(mat, x + 1, y),
          getMatVal(mat, x, y + 1), getMatVal(mat, x, y - 1))
    return sum([v != 0 for v in vv])


def trans(mat):
    newmat = [[s - 12 if s > 12 else 0 for s in r] for r in mat]
    for r in mat:
        for s in r:
            
    return newmat


def transLoop(mat):
    cnt = 0
    while True:
        mat = trans(mat)
        cnt += 1
        printMat(mat)
        break


def testcase(seq):
    row = int(raw_input())
    unused = int(raw_input())
    mat = []
    for i in range(row):
        line = raw_input()
        line = [int(i) for i in line.split()]
        mat.append(line)
    printMat(mat)
    transLoop(mat)
    print "Case #%d: " % (seq)


if __name__ == "__main__":
    n = int(raw_input())
    for i in range(n):
        testcase(i + 1)
