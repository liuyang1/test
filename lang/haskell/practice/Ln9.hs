-- 累积1/n，在n中不包含9的时候，是有限的
seq0 = [1..]

seq1 = filter (\x -> ('9' `elem` (show x))) seq0

lnn xs n = sum (map (\x -> 1.0 / (fromInteger x)) $ take n xs)

test n = (x, y, x - y, (x - y) / x)
    where x = lnn seq0 n
          y = lnn seq1 n
