double = (* 2)

-- [1, 0)
-- 错误，括号不匹配

-- double -3
-- 错误，(-3),显式错误应该是double不能应用到-这个函数

a = double (-3)
-- 正确

-- double double 0
-- 错误，函数是左结合的，因此会先计算dobule double
-- 而double函数不支持该类型输入

-- if 1 == 0 then 2==1
-- 错误，没有另一部分的else表达式部分

b = "++" == "+" ++ "+"
-- 正确，==的优先级较低

c :: [Integer -> Integer -> Integer]
c = [(+), (-)]
-- 正确，两个函数的类型都是Num a:: a -> a -> a

d = [[], [[]], [[[]]]]
-- ~~错误，列表中的类型必须一致~~
-- 类型为[[[[t]]]]

-- concat ["tea", "for", '2']
-- 错误，列表中的类型不一致

e = concat ["tea", "for", "2"]
-- 正确

main = do
        print a
        print b
        print $ c <*> [1, 2] <*> [1, 2]
        -- 必须加上类型，才可以输出
        print (d :: [[[[Integer]]]])
        print e
