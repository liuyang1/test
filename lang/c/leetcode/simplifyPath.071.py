class Solution(object):

    def simplifyPath(self, path):
        """
        :type path: str
        :rtype: str
        """
        ls = path.split('/')
        ls = [i for i in ls if i != "" and i != "."]
        rs = []
        for i in ls:
            if i == "..":
                if len(rs) != 0:
                    del rs[-1]
            else:
                rs.append(i)
        return "".join(["/" + i for i in rs])


def testcase(s, e):
    sln = Solution()
    r = sln.simplifyPath(s)
    print s, r, e, r == e


def test():
    testcase("/a/", "/a")
    testcase("/a//", "/a")
    testcase("/a///", "/a")
    testcase("/a////", "/a")
    testcase("/a//b", "/a/b")
    testcase("/a/./b", "/a/b")
    testcase("/a//b/../c/.d", "/a/c/.d")
    testcase("/a//b/../c/..d", "/a/c/..d")
    testcase("/a//b/../c/./d", "/a/c/d")
    testcase("/a/../b", "/b")
    testcase("/a/..", "/")
    testcase("/a/../", "/")
    testcase("/a/../b", "/b")
    testcase("/../", "/")
    testcase("/../a/b", "/a/b")
    testcase("/", "/")
    testcase("/em/jl///../.././../E/", "/E")
    testcase("/.././em/jl///../.././../E/", "/E")
    testcase("/.", "/")


if __name__ == "__main__":
    test()
