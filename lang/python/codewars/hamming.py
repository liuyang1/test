# final speedy solution
def mergeList2(l0, l1):
    lst = l0 + l1
    lst = set(lst)
    return sorted(list(lst))


glst = [1]
glst1 = []
def hamming(n):
    global glst, glst1
    n -= 1
    while 1:
        ret = glst[0]
        # store result at another list, easy
        glst1.append(ret)
        newlst = [ret * i for i in [2, 3, 5]]
        # only merge short list, fast
        glst = mergeList2(glst[1:], newlst)
        if n < len(glst1):
            return glst1[n]
