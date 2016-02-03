class Solution(object):

    def groupAnagrams(self, strs):
        """
        :type strs: List[str]
        :rtype: List[List[str]]
        >>> sln = Solution()
        >>> sln.groupAnagrams(["eta", "tea", "ate", "ant", "nat", "bat", "tab"])
        [['bat', 'tab'], ['ate', 'eta', 'tea'], ['ant', 'nat']]
        """
        from collections import defaultdict
        from itertools import groupby
        d = defaultdict(list)
        for k, g in groupby(strs, sorted):
            d[tuple(k)].extend(g)
        return [sorted(i) for i in d.values()]
