module DragonCurve where
import Data.Char

iter [] = []
iter ('a':xs) = "aRbFR" ++ iter xs
iter ('b':xs) = "LFaLb" ++ iter xs
iter (x:xs) = x: iter xs

dragon n
  | n < 0 = ""
  | otherwise = filter isUpper $ (iterate iter "Fa") !! n
