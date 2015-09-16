module Main where
--------------------------------------------------------------------------------
-- Lindenmayer
-- define Lindenmayer system, which generate fractal figure.
-- define
--   # axiom
--   # rule
--   # expand
-- Then, we got it.
--
-- HilbertCurve
-- ref: https://en.wikipedia.org/wiki/Hilbert_curve
-- DragonCurve is simlar

import System.Environment

lindenmayer ruleP axiomP n
  | n < 0 = ""
  | otherwise = iterate (>>= ruleP) axiomP !! n

rule 'a' = "LbFRaFaRFbL"
rule 'b' = "RaFLbFbLFaR"
rule x = [x]

axiom = "a"

-- expand to raw LOGO script
expand 'F' = "FD 6\n"
expand 'L' = "LT 90\n"
expand 'R' = "RT 90\n"
expand c = ""

hilbertCurve n = lindenmayer rule axiom n >>= expand

main = do args <- getArgs
          putStr . hilbertCurve . read $ head args
