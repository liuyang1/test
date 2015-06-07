module Codewars.Kata.SqIntoSq where
import Control.Applicative
import Data.Maybe
dech :: Integer -> Integer -> Maybe [Integer]
dech n p
  | p <= 0 = Nothing
  | p * p == n = Just [p]
  | otherwise = let d = n - p * p
                    v = dech d (min (floor (sqrt (fromInteger d))) (p - 1))
                 in (p:) <$> v <|> dech n (p - 1)
decompose :: Integer -> Maybe [Integer]
decompose n = reverse <$> dech (n ^ 2) (n - 1)
