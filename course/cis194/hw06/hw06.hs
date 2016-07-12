module Main where
--------------------------------------------------------------------------------
-- Exercise 1
fib 0 = 1
fib 1 = 1
fib n = fib (n - 1) + fib (n - 2)

fib1 = map fib [0..]

--------------------------------------------------------------------------------
-- Exercise 2
fib2 = [1, 1] ++ zipWith (+) fib2 (tail fib2)

data Stream a = Cons a (Stream a)

--------------------------------------------------------------------------------
-- Exercise 3
streamToList :: Stream a -> [a]
streamToList (Cons x s) = x: streamToList s

--------------------------------------------------------------------------------
-- Exercise 4

-- fmap :: Functor f => (a -> b) -> f a -> f b

--------------------------------------------------------------------------------
-- Exercise 5

sRepeat :: a -> Stream a
sRepeat x = Cons x (sRepeat x)

sIterate :: (a -> a) -> a -> Stream a
sIterate f a = Cons a (sIterate f (f a))

-- | interleave two streams
-- >>> take 6 $ streamToList $ sInterleave (sRepeat 0) (sRepeat 1)
-- [0,1,0,1,0,1]
sInterleave :: Stream a -> Stream a -> Stream a
sInterleave (Cons x sx) sy = Cons x (sInterleave sy sx)

-- | take n element from stream
-- >>> sTake 6 $ sRepeat 0
-- [0,0,0,0,0,0]
sTake :: Int -> Stream a -> [a]
sTake n = take n . streamToList

--------------------------------------------------------------------------------
-- Exercise 6

-- | natural number stream
-- >>> sTake 5 nats
-- [0,1,2,3,4]
nats :: Stream Integer
nats = sIterate (+ 1) 0

rulerIn i = sInterleave (sRepeat i) (rulerIn (i + 1))

-- | ruler, recursive on sInterleave
-- >>> sTake 10 ruler
-- [0,1,0,2,0,1,0,3,0,1]
ruler :: Stream Integer
ruler = rulerIn 0

--------------------------------------------------------------------------------
-- Exercise 7
randF x = (1103515245 * x + 12345) `mod` 2147483648

rand :: Int -> Stream Int
rand = sIterate randF

--------------------------------------------------------------------------------
-- Exercise 8
minMaxSlow :: [Int] -> Maybe (Int, Int)
minMaxSlow [] = Nothing
minMaxSlow xs = Just (minimum xs, maximum xs)

slow = print $ minMaxSlow $ sTake 1000000 $ rand 0

--------------------------------------------------------------------------------
-- Exercise 9
minMax :: [Int] -> Maybe (Int, Int)
minMax [] = Nothing
minMax (x:xs) = Just $ foldl f (x, x) xs
    where f (min, max) v
            | v < min = (v, max)
            | v > max = (min, v)
            | otherwise = (min, max)

-- slow use 205MB, fast use 160MB
fast = print $ minMax $ sTake 1000000 $ rand 0

--------------------------------------------------------------------------------
-- Exercise 10
data Matrix a = Matrix2 a a a a
    deriving (Show, Eq)

mulAdd a b c d = a * c + b * d
mulMat (Matrix2 a b c d) (Matrix2 x y z w) = Matrix2 (mulAdd a b x z) (mulAdd a b y w) (mulAdd c d x z) (mulAdd c d y w)
expMat m 1 = m
expMat m n
  | even n = mulMat mm mm
  | otherwise = mulMat m (expMat m (n - 1))
  where mm = expMat m (n `div` 2)

fastFib 0 = 1
fastFib n = a
    where Matrix2 a _ _ _ = expMat (Matrix2 1 1 1 0) n
