-- 可结合性
-- 数的加法，是可结合的。
-- 列表的拼接，是可结合的。
-- 函数的复合，也是可结合的。
--
-- 不可结合的例子
-- 指数运算
a = (2 ^ 3) ^ 5 /= 2 ^ (3 ^ 5)

-- 单位元
-- 加法的单位元，0
-- 列表拼接的单位元，[]
-- 函数复合的单位元，id

isUnit op e x = op e x == op x e

double x = 2 * x
isUnitOnVal op e x v = op e x v == op x e v
-- if check function equality, we must apply it to some value
-- otherwise, we cannot determine it.

-- isCatUnit = isUnit (++) []
-- It doesn't work, as we have to binding to specific type
-- so we cannot apply `isCatUnit` to different types

main = do
    print $ all (isUnit (+) 0) [1..10]
    print $ isUnit (++) [] "Hello"
    print $ isUnit (++) [] [1..10]
    print $ isUnit (++) [] [a]
    print $ isUnitOnVal (.) id double 21
