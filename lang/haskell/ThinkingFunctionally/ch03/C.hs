-- 定义失败，因为integral上的除法本身就是div运算
-- 或者说(/)要求参数为RealFrac类型，不是Integral类型的
-- div_wrong :: Integral a => a -> a -> a
div_wrong x y = floor (x / y)

div_correct :: Integral a => a -> a -> a
div_correct x y = floor (fromIntegral x / fromIntegral y)

test f = f <$> [(-3)..3] <*> (filter (/= 0) [(-3)..3])

corner f = f 1 0

main = print $ corner div_correct

-- 因为fromIntegral 0, 不是真正的0，而是一个非常接近于0的一个数
