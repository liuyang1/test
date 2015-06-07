module Snail where
import Data.List

snail :: [[Int]] -> [Int]
snail [] = []
snail (m:ms) = m ++ snail rm
  where rm = reverse $ transpose ms
