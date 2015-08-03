module DragonCurve where
import Data.Char

iter 'a' = "aRbFR"
iter 'b' = "LFaLb"
iter x = [x]

dragon n
  | n < 0 = ""
  | otherwise = filter isUpper $ (iterate (>>= iter) "Fa") !! n

-- generate logo script as dragon cruve
genLogo 'F' = "FD 6\n"
genLogo 'L' = "LT 90\n"
genLogo 'R' = "RT 90\n"

main = putStr $ (dragon 10 >>= genLogo)
