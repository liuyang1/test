def find(lst, p):
    return lst[p]


def union(lst, p, q):
    pid = find(lst, p)
    qid = find(lst, q)
    if (pid == qid):
        return
    for i in range(len(lst)):
        if lst[i] == pid:
            lst[i] = qid


def test():
    n = 10
    lst = [i for i in range(n)]
    pairs = [[6, 9], [8, 9], [8, 3], [0, 9], [0, 5], [3, 7]]
    for p, q in pairs:
        union(lst, p, q)
        # print p, q, lst
    for i in lst:
        print i,
    print


if __name__ == "__main__":
    test()
