module LeastCommonMultiple where
import Prelude hiding (lcm)

-- lcm :: Integral a => [a] -> a
lcm2 a b
  | a == 0 || b==0 = 0
  | otherwise = a * b `div` gcd a b
lcm xs
  | length xs <= 1 = head xs
  | otherwise = foldl1 lcm2 xs
