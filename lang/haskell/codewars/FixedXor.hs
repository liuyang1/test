module Codewars.Kata.FixedXor where

import Data.Bits (xor)
import Data.Char (digitToInt, intToDigit)
import Data.Function (on)

comb f g a b = f (g a b)

(.:) = ( . ) . ( . )

fixedXor :: String -> String -> String
fixedXor = zipWith (intToDigit .: (xor `on` digitToInt))
