class Solution(object):
    def combinationSum(self, candidates, target):
        """
        :type candidates: List[int]
        :type target: int
        :rtype: List[List[int]]
        >>> sln = Solution()
        >>> sln.combinationSum([1, 2, 3], 3)
        [[1, 1, 1], [1, 2], [3]]
        >>> sln.combinationSum([2, 3, 5, 6, 7, 10], 8)
        [[2, 2, 2, 2], [2, 3, 3], [2, 6], [3, 5]]
        """
        candidates = sorted(candidates)
        return self.combSum(candidates, target)

    def combSum(self, cand, target):
        """
        Divide two cases:
            - include cand[0], (when recursive, keep it to repeated choose)
            - not include cand[0]
        Then recursive
        """
        if len(cand) == 0 or target < 0:
            return []
        lst = []
        if cand[0] < target:
            lst += self.combSum(cand, target - cand[0])
        elif cand[0] == target:
            lst += [[]]
        [i.insert(0, cand[0]) for i in lst]
        others = self.combSum(cand[1:], target)
        return lst + others
