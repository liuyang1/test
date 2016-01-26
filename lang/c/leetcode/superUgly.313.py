import operator
from collections import deque


def ugly1(primes):
    qs = [deque([p]) for p in primes]
    while 1:
        idx, _ = min(enumerate([q[0] for q in qs]), key=operator.itemgetter(1))
        ret = qs[idx].popleft()
        for i in range(idx, len(qs)):
            qs[i].append(ret * primes[i])
        yield ret


class Solution(object):

    def __init__(self):
        self.mzlst = [1]

    def nthSuperUglyNumber(self, n, primes):
        """
        :type n: int
        :type primes: List[int]
        :rtype: int
        """
        self.mzUgly = ugly1(primes)
        if n < len(self.mzlst):
            return self.mzlst[n - 1]
        for i in xrange(len(self.mzlst), n):
            self.mzlst.append(self.mzUgly.next())
        return self.mzlst[n - 1]


if __name__ == "__main__":
    sln = Solution()
    # print sln.nthSuperUglyNumber(20, [2, 7, 13, 19])
    print sln.nthSuperUglyNumber(100000, [7,19,29,37,41,47,53,59,61,79,83,89,101,103,109,127,131,137,139,157,167,179,181,199,211,229,233,239,241,251])
