-- list is builtin Monad
-- 2016-01-24 - compare BIND vs. list comprehension

xs = [0..5]

expand x = [x * x, x * 2 + 1]
r0 = xs >>= expand

fs = [\x -> x * x, \x -> x * 2 + 1]
r1 = [f x | x <- xs, f <- fs]

-- >>=
-- bind operator
-- >>= :: m a -> (a -> m b) -> m b
-- >>= :: [a] -> (a -> [b]) -> [b]
main = do
        print r0
        print r1
