class Solution(object):

    def combinationSum3(self, k, n):
        """
        :type k: int
        :type n: int
        :rtype: List[List[int]]
        reuse combinationSum2, just filter solution whose length is K
        >>> sln = Solution()
        >>> sln.combinationSum3(3, 7)
        [[1, 2, 4]]
        >>> sln.combinationSum3(3, 9)
        [[1, 2, 6], [1, 3, 5], [2, 3, 4]]
        """
        candidates = [i for i in range(9, 0, -1)]
        lst = self.combSum3(candidates, n)
        return [i for i in lst if len(i) == k]

    def combSum3(self, cand, target):
        """
        Divide two cases:
            - include cand[0]
            - not include any equal to cand[0]
        Then recursive
        """
        if len(cand) == 0 or target < 0:
            return []
        lst = []
        if cand[0] < target:
            lst += self.combSum3(cand[1:], target - cand[0])
        elif cand[0] == target:
            lst += [[]]
        [i.append(cand[0]) for i in lst]
        others = self.combSum3(cand[1:], target)
        return lst + others
