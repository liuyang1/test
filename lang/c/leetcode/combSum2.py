class Solution(object):
    def combinationSum2(self, candidates, target):
        """
        :type candidates: List[int]
        :type target: int
        :rtype: List[List[int]]
        >>> sln = Solution()
        >>> sln.combinationSum2([1, 1, 2, 3], 3)
        [[3], [1, 2]]
        >>> sln.combinationSum2([1, 2, 1, 5, 6, 7, 10], 8)
        [[1, 7], [2, 6], [1, 1, 6], [1, 2, 5]]
        """
        candidates = sorted(candidates, reverse=True)
        return self.combSum2(candidates, target)

    def combSum2(self, cand, target):
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
            lst += self.combSum2(cand[1:], target - cand[0])
        elif cand[0] == target:
            lst += [[]]
        [i.append(cand[0]) for i in lst]
        rmFirst = [i for i in cand if i != cand[0]]
        others = self.combSum2(rmFirst, target)
        return lst + others
