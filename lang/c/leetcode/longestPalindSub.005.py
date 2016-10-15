#! /usr/bin/env python2

"""
find longest palindrom substring
https://leetcode.com/problems/longest-palindromic-substring/

ref:
- http://taop.marchtea.com/01.05.html
- https://en.wikipedia.org/wiki/Longest_palindromic_substring

"""
def bestSeq(n):
    # use bestSeq will find most potential longest substring,
    # but will return wrong answer when want to find FIRST substring
    l = range(n)
    o = [i for i in l if i % 2 == 1]
    e = [i for i in l if i % 2 == 0]
    r = [v for p in zip(o[::-1], e[::-1]) for v in p]
    if len(o) != len(e):
        r.append(e[0])
    return r


def longestPalindrome_1(s):
    """
    O(n ^ 2) solution
    """
    if len(s) == 0:
        return s
    mp, ml = 0, 0
    for i in range(0, len(s)):
        l = -1
        for r in range(0, len(s)):
            if not (i - r >= 0 and i + r < len(s) and s[i - r] == s[i + r]):
                break
            p, l = i - r, 2 * r + 1
        if l > ml:
            mp, ml = p, l
        for r in range(0, len(s)):
            if not (i - r >= 0 and i + r + 1 < len(s) and s[i - r] == s[i + r + 1]):
                break
            p, l = i - r, 2 * r + 2
        if l > ml:
            mp, ml = p, l
        if ml == len(s):
            break
    return s[mp: mp + ml]


class Solution(object):

    def longestPalindrome(self, s):
        return longestPalindrome_1(s)

    def check(self, s, e):
        r = self.longestPalindrome(s)
        if r == e:
            # print "longestPalindrome('%s')='%s' == '%s' SUCC" % (s, r, e)
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
    # must return first substring
    sln.check("abcda", "a")


def testPerf():
    sln = Solution()
    s = "a" * 1000
    sln.check(s, s)


if __name__ == "__main__":
    testPositive()
    testPerf()
