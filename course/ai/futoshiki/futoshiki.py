"""
https://www.mindgames.com/game/Futoshiki

Futoshiki 是日本一款类似于数组 Suduko的游戏
限定是同行同列1-5各出现一次（类似于拉丁方）
并且满足相邻元素之间大小限制，如图sample
"""

"""
解决思路
尝试，
根据Sokudu规则发现可选集，从可选集合最小开始尝试（减少潜在尝试次数）
尝试，发现
1. 冲突，未设定的位置，找不到可选集合，退出，回溯尝试
2. 全都选择匹配上，则打印结果
"""


class board():
    def show(self):
        for i in self.xss:
            print(i)

    def __init__(self, xss, groups):
        self.xss = xss
        self.groups = groups

    def get(self, x, y):
        return self.xss[x][y]

    def set(self, x, y, v):
        self.xss[x][y] = v

    def unset(self, x, y):
        self.xss[x][y] = 0  # 0 表明这里未填写元素

    def candSokudu(self, x, y):
        v = self.get(x, y)
        if v != 0:
            return [v]
        lst0 = [self.get(x, i) for i in range(5)]  # 棋盘大小固定为5x5
        lst1 = [self.get(i, y) for i in range(5)]
        # 元素固定为1到5
        return [a for a in range(1, 6) if a not in lst0 and a not in lst1]

    def less(self, x0, y0, x1, y1):
        v0 = self.get(x0, y0)
        v1 = self.get(x1, y1)
        if v0 != 0 and v1 != 0:
            return v0 < v1
        return None

    def constraint(self):
        checks = [self.less(x0, y0, x1, y1) for x0, y0, x1, y1 in self.groups]
        for c in checks:
            if c == False:
                return False
        return True

    def tryloop(self):
        # self.show()
        mx, my, ml = -1, -1, 6
        for x in range(5):
            for y in range(5):
                if self.get(x, y) == 0:
                    cands = self.candSokudu(x, y)
                    if len(cands) < ml:
                        ml, mx, my = len(cands), x, y
        if ml == 0:  # conflict after try
            return False
        if mx == -1 and my == -1:  # find one solution
            self.show()
            return True
        # print(mx, my, self.candSokudu(mx, my))
        cands = self.candSokudu(mx, my)
        for v in cands:
            self.set(mx, my, v)
            if self.constraint():
                if self.tryloop() == True:
                    pass
            self.unset(mx, my)
        return False  # try all cands, still fail


def test():
    xs = [[0, 0, 0, 4, 0],
          [0, 0, 0, 0, 0],
          [0, 0, 0, 0, 2],
          [0, 2, 0, 0, 0],
          [0, 0, 5, 2, 0]]
    groups = [(0, 1, 1, 1),
              (0, 2, 1, 2),
              (2, 3, 2, 4),
              (2, 4, 3, 4),
              (3, 3, 3, 4),
              (3, 2, 4, 2),
              (4, 1, 4, 0),
              (4, 4, 3, 4)]
    brd = board(xs, groups)
    # print(brd.get(2, 3))
    # print(brd.get(0, 3))
    # print(brd.candSokudu(0, 0))
    brd.tryloop()


def test2():
    xs = [[3, 0, 0, 0, 0],
          [0, 0, 5, 0, 0],
          [0, 0, 0, 0, 0],
          [0, 0, 4, 0, 0],
          [0, 0, 0, 0, 0]]
    groups = [(0, 1, 0, 2),
              (0, 4, 0, 3),
              (1, 0, 2, 0),
              (1, 1, 2, 1),
              (1, 3, 1, 2),
              (1, 4, 0, 4),
              (1, 4, 2, 4),
              (2, 2, 1, 2),
              (3, 1, 2, 1),
              (3, 4, 4, 4),
              (4, 1, 4, 0)]
    brd = board(xs, groups)
    brd.tryloop()


if __name__ == "__main__":
    test2()
