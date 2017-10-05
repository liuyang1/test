f0 = any p
f1 = not . all (not p)
-- ~~f0, f1 不相等~~
-- 也成立
-- f0 = any p -> 逆命题 all (not p) -> 逆否命题 not . all (not p)

-- 列表的笛卡尔积
cp [] = [[]]
cp (xs:xss) = [x: ys | x <- xs, ys <- yss]
    where yss = cp yss

g0 = any null
g1 = null . cp
-- 相等
