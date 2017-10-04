-- newton法求平方根
-- 0 会陷入死循环，因为始终无法真正迭代到零
mysqrt 0 = 0
mysqrt x = guess 1
    where eps = 0.000001
          guess i
             | abs (i * i - x) < eps * x = i
             | otherwise = guess ((i + (x / i)) / 2)

-- until 这个写法棒啊
-- 整个过程拆分成相对独立而有意义的几个函数
-- SICP里面曾经介绍过，自己居然不记得了
mysqrt1 0 = 0
mysqrt1 x = until goodEnough improve x
    where goodEnough i = abs (i * i - x) < eps * x
          improve y = (y + x / y) / 2
          eps = 0.000001

main = do
        print $ map mysqrt [0..5]
        print $ map mysqrt1 [0..5]
