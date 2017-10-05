f0 p xs ys = [(x, y) | x <- xs, p x, y <- ys]
f1 p xs ys = [(x, y) | x <- xs, y <- ys, p x]

g0 p xs ys = zip (filter p xs) ys
g1 p xs ys = filter (\(x, _) -> p x) $ zip xs ys

-- ~~当 map p xs = [] || length ys <= 1 的时候，二者结果相同~~
-- ~~否则结果不同~~

-- 只有当ys是有穷列表的时候，二者才结果不同
-- 前者因为不需要展开ys,因此可以计算出有穷的结果

assertSame p xs ys = f0 p xs ys == f1 p xs ys
assertDiff p xs ys = not $ assertSame p xs ys

main = do
        print $ assertSame even [1] [1..5]
        print $ assertSame even [2, 4] [1]
        print $ assertSame even [2, 4] ([]::[Int])
        print $ assertDiff even [2, 3] [1, 2]
