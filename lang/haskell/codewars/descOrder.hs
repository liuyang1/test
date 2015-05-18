module DescendingOrder where
import Data.List

digith 0 xs = xs
digith n xs = digith (n `div` 10) (rem n 10 : xs)
digit n = digith n []

combh [] n = n
combh (x: xs) n = combh xs $ n * 10 + x
comb xs = combh xs 0

descendingOrder :: Integer -> Integer
descendingOrder n = comb (reverse (sort (digit n)))
