module Main where

import System.Environment
import Data.List (maximumBy)
import Data.Function (on)

prices = [1, 5, 8, 9, 10, 17, 17, 20, 24, 30]

cutRod0 ps n
  | n == 0 = 0
  | n > 0 = maximum $ map cut [1..(length ps)]
  | otherwise = undefined
  where cut m = if n >= m then ps !! (m - 1) + cutRod0 ps (n - m)
                         else 0

-- cutRod1 :: [Integer] -> Int -> Integer
cutRod1 ps = (map f [0 ..] !!)
    where f 0 = 0
          f n = maximum $ map (cut n) [1..(length ps)]
          cut n m = if n >= m then ps !! (m - 1) + cutRod1 ps (n - m)
                              else 0

cutRod2 ps n = go [] 0
    where l = length ps
          go xs i = if i == n then xs else go (x: xs) (i + 1)
              where x = maximum $ [ps !! i | i < l] ++ zipWith (+) xs ps

-- 15.1-3 consider fixed cost on cuting every time
c = 1

cutRod3 ps n = go [] 0
    where l = length ps
          go xs i = if i == n then xs else go (x: xs) (i + 1)
              where x = maximum $ [ps !! i | i < l] ++ zipWith (+) ps (map (c -) xs)

-- 15.1.-4 return solution
-- [(value, [slices]), ..]

data Sln = Sln Int [Int] deriving (Show, Eq)

instance Ord Sln where
    compare (Sln v0 _) (Sln v1 _) = compare v0 v1

add :: Sln -> Sln -> Sln
add (Sln a x) (Sln b y) = Sln (a + b) (x++y)

up a b = Sln a [b]

cutRod4 ps n = go [] 0
    where l = length ps
          ps_ = zipWith up ps [1..]
          go xs i = if i == n then xs else go (x: xs) (i + 1)
              where x = maximum $ [ps_ !! i | i < l] ++ zipWith add ps_ xs

main = do args <- getArgs
          print $ cutRod1 prices $ read $ head args
