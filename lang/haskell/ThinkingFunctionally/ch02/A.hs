half x = x / 2

-- 二加二的一半是二还是三？
-- 优先级的问题
-- 依次顺序为
-- - 函数调用
-- - / 除法
-- - + 加法
-- 因此，在没有显式指明顺序的情况下，是3
main = do
        print $ 2 + 2 / 2
        print $ 2 + half 2
        print $ 2 + ($ 2) half
        print $ (2 + 2) / 2
        print $ half (2 + 2)
