module Codewars.Kata.FixedXor where

import Data.Bits (xor)
import Data.Char (digitToInt, intToDigit)
import Data.Function (on)

(.:) = (.) . (.)

fixedXor :: String -> String -> String
fixedXor = zipWith (intToDigit .: (xor `on` digitToInt))
