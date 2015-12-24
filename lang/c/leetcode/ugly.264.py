def ugly1():
    q2, q3, q5 = [2], [3], [5]
    while 1:
        # print q2, q3, q5
        if q2[0] < q3[0] and q2[0] < q5[0]:
            ret = q2.pop(0)
            q2.append(ret * 2)
            q3.append(ret * 3)
            q5.append(ret * 5)
        elif q3[0] < q2[0] and q3[0] < q5[0]:
            ret = q3.pop(0)
            q3.append(ret * 3)
            q5.append(ret * 5)
        else:
            ret = q5.pop(0)
            q5.append(ret * 5)
        yield ret


class Solution(object):

    def __init__(self):
        self.mzlst = [1]
        self.mzUgly = ugly1()

    def nthUglyNumber(self, n):
        """
        :type n: int
        :rtype: int
        """
        if n < len(self.mzlst):
            return self.mzlst[n - 1]
        for i in xrange(len(self.mzlst), n):
            self.mzlst.append(self.mzUgly.next())
        return self.mzlst[n - 1]
