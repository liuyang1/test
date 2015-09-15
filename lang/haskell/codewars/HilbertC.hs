-- module HilbertCurve where
module Main where

--------------------------------------------------------------------------------
-- HilbertCurve
-- ref: https://en.wikipedia.org/wiki/Hilbert_curve

import Data.Char

iter 'a' = "LbFRaFaRFbL"
iter 'b' = "RaFLbFbLFaR"
iter x = [x]

hilbert n
  | n < 0 = ""
  | otherwise = filter isUpper $ (iterate (>>= iter) "a") !! n

-- genrate logo script as hilbert curve
genLogo 'F' = "FD 20\n"
genLogo 'L' = "LT 90\n"
genLogo 'R' = "RT 90\n"
genLogo c = ""

main = putStr $ (hilbert 4 >>= genLogo)
