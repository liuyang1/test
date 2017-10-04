exp0 _ 0 = 1
exp0 x 1 = x
exp0 x n = x * exp0 x (n - 1)
-- O(n) times

exp1 _ 0 = 1
exp1 x 1 = x
exp1 x n | even n = y * y
         | otherwise = y * y * x
         where y = exp1 x (n `quot` 2)

-- O(log(n)) times

test = all (\x -> (exp0 2 x) == (exp1 2 x)) [0..10]

main = print test
