import Data.List

double :: Integer -> Integer
double x = 2 * x

lst = [1, 4, 4, 3]
test0 = do
    print $ map double lst
    print $ map (double . double) lst -- function composing
    print $ map double []

-- test function is equal or not on list
eqFunc f g = f lst == g lst

-- test function is equal or not on list of list
eqFunc' f g = f xs == g xs
    where xs = [[1, 4], [4, 3], [5]]

test1 = do
    print $ eqFunc (sum . map double) (double . sum)
    print $ eqFunc' (sum . map sum) (sum . concat)
    print $ eqFunc (sum . sort) sum

-- sum . map double == double . sum
-- 2 * x0 + 2 * x1 + ... = 2 * (x0 + x1 + ...)
-- 乘法的分配律，决定该等式成立
--
-- sum . map sum == sum . concat
-- (x00 + x01 + ...) + (x10 + x11 + ...) + ... == x00 + x01 + ... + x10 + x11 + ...
-- 加法的结合律，决定该等式成立
--
-- sum == sum . sort
-- x0 + x1 + ... = y0 + y1 + ...
-- [y]是[x]排序后的结果
-- 加法的结合律，决定该等式成立

main = do
    test0
    test1
