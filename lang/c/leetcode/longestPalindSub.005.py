#! /usr/bin/env python2

"""
find longest palindrom substring
https://leetcode.com/problems/longest-palindromic-substring/

ref:
- http://taop.marchtea.com/01.05.html
- https://en.wikipedia.org/wiki/Longest_palindromic_substring

"""

import operator


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


def longestPalindrome(s):
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


def manacher(s):
    """
    complexity analysis:
    notice MX only from 0 to N, rightmost always move to right ascending.
    so O(N)
    """
    # interleave with "#" for handle odd substring and even string two cases
    s = [i for j in zip("#" * len(s), s) for i in j]
    s = "".join(s)
    s += "#"
    ln = len(s)

    # p: centered with s[i]
    # cen: center index of rightmost substring from 0 to now(i)
    # mx: rightmost position + 1 of longest substring from 0 to now(i)
    p = [0] * ln
    mx, cen = 0, 0
    for i in range(1, ln):
        # mx > i: longest substring cover new i position
        # example: #  c  #  a  #  b  #  a  #  b  #  a  #
        # p        0  2  1  2  1  4  1  -  -  -  -  -  -
        #                   ^     ^     ^     ^
        #                   |    CEN    i     MX
        #                2*CEN-i
        # -  i and 2*CEN-i is palindrome with CEN
        # -  from [2*CEN-MX, MX] is palindrome
        # => left substring is palindrome, right substring also palindrome
        # so we could save a lot of comparison now.
        p[i] = min(p[2 * cen - i], mx - i) if mx > i else 1
        # continue to extend substring with palindrome checking
        while i + p[i] < ln and i - p[i] >= 0 and s[i + p[i]] == s[i - p[i]]:
            p[i] += 1
        # if new substring is more right, continue to use it.
        if i + p[i] > mx:
            mx = i + p[i]
            cen = i
    # get longest one, and its index
    idx, radius = max(enumerate(p), key=operator.itemgetter(1))
    radius -= 1  # original radius conclude center point, so remove it
    ret = s[idx - radius: idx + radius + 1]
    ret = [i for i in ret if i != '#']  # filter out #
    ret = "".join(ret)
    return ret


class Solution(object):

    def longestPalindrome(self, s):
        return manacher(s)

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
    s = "detartrated"
    sln.check(s, s)
    s = "sator arepo tenet opera rotas"
    sln.check(s, s)


def testPerf():
    sln = Solution()
    s = "a" * 1000
    sln.check(s, s)


def test():
    testPositive()
    testPerf()


def profile():
    import cProfile
    cProfile.run('test()')


if __name__ == "__main__":
    test()
