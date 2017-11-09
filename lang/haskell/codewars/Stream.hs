module Stream where

import Control.Arrow
import Control.Applicative

-- import Stream.Internal

-- Defined in Stream.Internal:
data Stream a = a :> Stream a deriving (Show)

infixr :>

-- | Get the first element of a stream.
headS :: Stream a -> a
headS (x :> s) = x

-- | Drop the first element of a stream.
tailS :: Stream a -> Stream a
tailS (x :> s) = s


-- {{{ Stream constructors

-- | Construct a stream by repeating a value.
repeatS :: a -> Stream a
repeatS x = xs
    where xs = x :> xs

-- | Construct a stream by repeatedly applying a function.
iterateS :: (a -> a) -> a -> Stream a
iterateS f x = x :> iterateS f (f x)

-- | Construct a stream by repeating a list forever.
cycleS :: [a] -> Stream a
cycleS xs = h xs
    where h = foldr (:>) (cycleS xs)

-- | Construct a stream by counting numbers starting from a given one.
fromS :: Num a => a -> Stream a
fromS x = x :> fromS (succ x)
    where succ i = i + 1

-- | Same as 'fromS', but count with a given step width.
fromStepS :: Num a => a -> a -> Stream a
fromStepS x s = x :> fromStepS (succ x) s
    where succ i = i + s

-- }}}


-- | Fold a stream from the left.
foldrS :: (a -> b -> b) -> Stream a -> b
foldrS f (x :> xs) = f x (foldrS f xs)

-- | Filter a stream with a predicate.
filterS :: (a -> Bool) -> Stream a -> Stream a
filterS p (x :> xs)
    | p x = x :> filterS p xs
    | otherwise = filterS p xs

mapS :: (a -> b) -> Stream a -> Stream b
mapS f (x :> s) = f x :> mapS f s

-- | Take a given amount of elements from a stream.
takeS :: Int -> Stream a -> [a]
takeS i (x :> xs)
    | i <= 0 = []
    | otherwise = x: takeS (i - 1) xs

-- | Drop a given amount of elements from a stream.
dropS :: Int -> Stream a -> Stream a
dropS i xx@(x :> xs)
    | i <= 0 = xx
    | otherwise = dropS (i - 1) xs

-- | Do take and drop simultaneous.
splitAtS :: Int -> Stream a -> ([a], Stream a)
splitAtS i s = (takeS i s, dropS i s)
-- XXX: two runs

-- | Combine two streams with a function.
zipWithS :: (a -> b -> c) -> Stream a -> Stream b -> Stream c
zipWithS f (x :> xs) (y :> ys) = f x y :> zipWithS f xs ys

zipS :: Stream a -> Stream b -> Stream (a, b)
zipS = zipWithS (,)

instance Functor Stream where
    -- fmap :: (a -> b) -> Stream a -> Stream b
    fmap f (x :> xs) = f x :> fmap f xs

instance Applicative Stream where
    -- pure :: a -> Stream a
    pure = repeatS

    -- (<*>) :: Stream (a -> b) -> Stream a -> Stream b
    (<*>) = zipWithS id

-- | The stream of fibonacci numbers.
fibS :: Stream Integer
fibS = 0 :> 1:> zipWithS (+) fibS (tailS fibS)

-- | The stream of prime numbers.
primeS :: Stream Integer
primeS = sieve (fromS 2)
    where sieve (x :> xs) = x :> sieve (filterS (\i -> i `mod` x /= 0) xs)

factorFreq n p
    | n `mod` p /= 0 = 0
    | otherwise = 1 + factorFreq (n `div` p) p

-- 质数质数表示法
primeFactorS :: Integer -> Stream Integer
-- primeFactorS n = mapS (factorFreq n) primeS
primeFactorS n = mapS (factorFreq n) primeS

a = takeS 10 $ primeFactorS 112
