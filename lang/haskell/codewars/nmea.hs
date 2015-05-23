module NMEA where
import Data.List
import Data.Bits
import Data.Char

dropUntil fn xs = tail $ dropWhile fn xs
digitlst2Num ds = foldl (\a b -> (16 * a) + b) 0 $ map digitToInt ds
check :: String -> Bool
check s =  all isHexDigit cs && digitlst2Num cs == foldl1 xor $ map ord ms
  where ss = dropUntil (/= '$') s
        ms = takeWhile (/= '*') ss
        cs = takeWhile (/= '\r') $ dropUntil (/= '*') ss
