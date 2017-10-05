-- 两个递减序列，是否含有公共元素
disjoint :: (Ord a) => [a] -> [a] -> Bool
disjoint xx@(x:xs) yy@(y:ys) = case compare x y of
                                LT -> disjoint xs yy
                                EQ -> True
                                GT -> disjoint xx ys
disjoint _ _ = False -- 任意一个为空的时候

-- disjoint 必须满足交换律
test e a b = disjoint a b == e && disjoint b a == e

-- []没有类型，需要指明
main = do
        print $ test False ([]::[Int]) ([]::[Int])
        print $ test False [1] []
        print $ test False [1, 2] [3, 4]
        print $ test True [1..10] [3]
