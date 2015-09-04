module Codewars.Kata.PP (isPP) where
--------------------------------------------------------------------------------
--  Perfect Power
--  Perfect Power is number which can represent as m ^ k, which m, k ∈ ZZ
--
--  This kata, is to determine a number is (or not) a Perfect Power.
--  if Yes, return Just (m, k), only one valid group (m, k) is ok.
--  else, return Nothing
--
--  Algo:
--  # For specific power k, we can binary search.
--  # Serach power k, number [2, log(n)]. can opt to prime number
--
--  Complexity:
--  binary search is log(n), k from [2, log(n)],
--  Complexity is log²(n)
import Data.Maybe

pps n k bgn end
  | v == n = Just (mid, k)
  | bgn == mid = Nothing
  | mid == end = Nothing
  | v > n = pps n k bgn mid
  | otherwise = pps n k mid end
  where mid = ((bgn + end) `div` 2)
        v = mid ^ k

ppss n k0
  | 2 ^ k0 > n = Nothing
  | isJust r = r
  | otherwise = ppss n (k0 + 1)
  where r = pps n k0 2 (n `div` k0)

isPP :: Integer -> Maybe (Integer, Integer)
isPP n = ppss n 2
