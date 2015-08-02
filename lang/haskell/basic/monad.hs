-- list is buildin Monad

xs = [0..5]
expand x = [2 * x, 2 * x + 1]

-- >>=
-- bind operator
-- >>= :: m a -> (a -> m b) -> m b
-- >>= :: [a] -> (a -> [b]) -> [b]
main = print $ xs >>= expand
