module Hamming where

hamming :: String -> String -> Int
hamming a b = sum . map differ $ zip a b
  where differ (aa, bb) = if aa == bb then 0 else 1
