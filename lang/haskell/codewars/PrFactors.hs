module Codewars.Kata.PrFactors where

import Data.List (group)
import Control.Arrow ((&&&))

-- In Haskell, I prefer to use recursive style as it's lazy.
primeFsHlp 1 _ = []
primeFsHlp n p
  | (d, 0) <- n `divMod` p = p: primeFsHlp d p
  | p * p > n = [n]-- key to speed of this function
  | otherwise = primeFsHlp n (p + 1)

primeFs :: Integral a => a -> [(a, Int)]
primeFs 1 = [(1, 1)]
primeFs n = map (head &&& length) . group $ primeFsHlp n 2

prime_factors :: Integer -> String
prime_factors = concatMap fmt . primeFs
    where showBase 1 = ""
          showBase b = "**" ++ show b
          fmt (n, b) =  "(" ++ show n ++ showBase b ++ ")"

main = mapM_ (print . prime_factors) $ [1234566, 1232347587] ++ [1..20]
