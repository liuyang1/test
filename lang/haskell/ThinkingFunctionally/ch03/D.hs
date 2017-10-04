floor_dick :: Float -> Integer
floor_dick = read . (takeWhile (/= '.')) . show

-- 因为floor_dick在负数范围内不对
-- 而且对于科学计数法，就完全失败了
