def callIter(s, init, iterFn, retFn):
    lst, r = init(s)
    if len(lst) == 0:
        return ""
    while True:
        one = lst[0]
        newlst = iterFn(s, lst, r)
        if len(newlst) == 0:
            r -= 1
            return retFn(s, one, r)
        lst = newlst
        r += 1


def searchOdd(s):
    def init(s):
        return [i for i in range(0, len(s))], 1

    def iterOdd(s, lst, r):
        def isSameRadius(i):
            return i - r >= 0 and i + r < len(s) and s[i - r] == s[i + r]
        return [i for i in lst if isSameRadius(i)]

    def retOdd(s, m, r):
        return s[m - r: m + r + 1]
    return callIter(s, init, iterOdd, retOdd)


def searchEven(s):
    def init(s):
        return [i for i in range(0, len(s) - 1)], 0

    def iterEven(s, lst, r):
        def isSameRadius(i):
            return i - r >= 0 and i + r + 1 < len(s) and s[i - r] == s[i + r + 1]
        return [i for i in lst if isSameRadius(i)]

    def retEven(s, m, r):
        return s[m - r: m + r + 2]
    return callIter(s, init, iterEven, retEven)


def searchLongstPalind(s):
    odd = searchOdd(s)
    even = searchEven(s)
    return odd if len(odd) > len(even) else even


class Solution(object):

    def longestPalindrome(self, s):
        return searchLongstPalind(s)

    def check(self, s, e):
        r = self.longestPalindrome(s)
        if r == e:
            return True
        print "longestPalindrome('%s')='%s' != '%s' FAIL" % (s, r, e)
        exit(-1)


def testPositive():
    sln = Solution()
    sln.check("a", "a")
    s = "abcdefedcba"
    sln.check(s, s)
    sln.check(s + "aaaa", s)
    sln.check("aaaa" + s, s)
    sln.check("aa", "aa")
    sln.check("abba", "abba")
    s = "abcddddcba"
    sln.check(s, s)
    sln.check(s + "aaaa", s)
    sln.check("aaaa" + s, s)
    sln.check("abac", "aba")


def testPerf():
    sln = Solution()
    s = "a" * 1000
    sln.check(s, s)


if __name__ == "__main__":
    testPositive()
    testPerf()
