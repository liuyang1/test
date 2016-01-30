def find(ids, p):
    while p != ids[p]:
        p = ids[p]
    return p

def union(ids, sz, p, q):
    i = find(ids, p)
    j = find(ids, q)
    if i == j:
        return
    if sz[i] < sz[j]:
        ids[i] = j
        sz[j] += sz[i]
    else:
        ids[j] = i
        sz[i] += sz[j]


def test():
    n = 10
    lst = [i for i in range(n)]
    sz = [1 for i in range(n)]
    pairs = [[7, 3], [9, 1], [4, 0], [0, 1], [7, 5], [6, 3], [5, 2], [6, 9], [1, 8]]
    for p, q in pairs:
        union(lst, sz, p, q)
    for i in lst:
        print i,
    print


if __name__ == "__main__":
    test()
