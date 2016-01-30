module Main where
-- module Dice where

-- non-rec faster 50%
choose n 0 = 1
choose 0 k = 0
choose n k = product [(n - k + 1)..n] `div` product [1..k]

dice s n k = sum $ map fn [0..((k - n) `div` s)]
  where fn i = ((-1) ^ i) * (choose n i) * (choose (k - s * i - 1) (n - 1))

-- this rec version is slow
bidice s 1 k
  | k >= 1 && k <= s = 1
  | otherwise = 0
bidice s n k
  | even n = sum $ zipWith (*) ll (reverse ll)
  | otherwise = sum [bidice s (n - 1) i | i <- [1..(k - 1)]]
  where ll = [bidice s (n `div` 2) i | i <- [1..(k - 1)]]

-- by choose func, remove redutant lst calc, faster 15%
cdice s n kk = sum $ map (sum . (zipWith (*) lst0)) lst1
  where lst0 = [((-1) ^ i) * (choose n i) | i <- [0..((kk -n) `div` s)]]
        lst1 = [[choose (k - s * i - 1) (n - 1) | i <- [0..((k - n) `div` s)]] | k <- [0..(kk - 1)]]

rollDice :: Integer -> Integer -> Integer -> Double
rollDice n s k
  | 2 * k > thr = 1 - rollDice n s (thr - k)
  | otherwise = 1 - toDouble (cdice s n k) / toDouble s ** toDouble n
  where thr = n * (s + 1)
        toDouble = fromRational . fromIntegral

main = print $ map (rollDice 20 99) [1000..1200]
