module Codewars.Kata.Decompose where
import Data.Ratio

egy :: (Integral a) => a -> a -> [(a, a)]
egy a b
    | a == 0 = []
    | a < b = (1, den): (egy (a * den - b) (b * den))
    | otherwise = (num, 1): (egy (a - b * num) b)
    where den = b `div` a + (if (b `mod` a == 0) then 0 else 1)
          num = (a `div` b)

decompose :: String -> String -> [String]
decompose n d = map showFrac $ egy (read n) (read d)
    where showFrac (x, 1) = show x
          showFrac (1, x) = "1/" ++ show x

main = do
    print $ decompose "5" "6"
    print $ decompose "0" "2"
    print $ decompose "3" "4"
    print $ decompose "12" "4"
    print $ decompose "21" "23"
    print $ decompose "14" "3"
    print $ decompose "2" "1000000000"
    print $ decompose "1000000000" "2"
    print $ decompose "5" "4"
