class Solution(object):
    def reverseWords(self, s):
        """
        :type s: str
        :rtype: str
        >>> sln = Solution()
        >>> sln.reverseWords("the sky is blue")
        'blue is sky the'
        >>> sln.reverseWords(" ")
        ''
        """
        return " ".join(filter(lambda x: x != '', reversed(s.split(" "))))
