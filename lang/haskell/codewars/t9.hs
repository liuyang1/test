module Haskell.Codewars.KeypadEntry where
import Data.Char
import Data.List
import Data.Maybe

keys = ["1", "ABC2", "DEF3", "GHI4", "JKL5", "MNO6",
       "PQRS7", "TUV8", "WXYZ9", "*", " 0", "#"]

fkey (k:ks) x = if x `elem` k
                   then fromJust (elemIndex x k) + 1
                   else fkey ks x
fkey' = fkey keys . toUpper

presses :: String -> Int
presses = sum . map fkey'
