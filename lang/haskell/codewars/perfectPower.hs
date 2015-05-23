module Codewars.Kata.PP (isPP) where
import Data.List
import Control.Arrow

sieve (p:xs) = p : sieve [x | x <- xs, x `mod` p /= 0]
primes = sieve [2..]
factor 1 xs ps = xs
factor n xs (p:ps)
  | rem n p == 0 = factor (n `div` p) (p:xs) (p:ps)
  | otherwise = factor n xs ps
factorg n = map (head &&& length) . group $ factor n [] primes
gcdl [] = 1
gcdl xs = foldl1 gcd xs
resl fs ps g = product . map (uncurry (^)) $ zip fs (map (`div` g) ps)
isPP :: Integer -> Maybe (Integer, Integer)
isPP n
  | length ps < 1 = Nothing
  | g /= 1 = Just (resl fs ps g, toInteger g)
  | otherwise = Nothing
    where (fs, ps) = unzip $ factorg n
          g = gcdl ps
