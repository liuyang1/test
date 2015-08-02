-- ref: http://www.codewars.com/kata/base-conversion
module BaseConversion where

import Data.List
import Data.Maybe

newtype Alphabet = Alphabet { getDigits :: [Char] } deriving (Show)
bin, oct, dec, hex, alphaLower, alphaUpper, alpha, alphaNumeric, base64 :: Alphabet
bin = Alphabet $ "01"
oct = Alphabet $ ['0'..'7']
dec = Alphabet $ ['0'..'9']
hex = Alphabet $ ['0'..'9'] ++ ['a'..'f']
alphaLower = Alphabet $ ['a'..'z']
alphaUpper = Alphabet $ ['A'..'Z']
alpha = Alphabet $ ['a'..'z'] ++ ['A'..'Z']
alphaNumeric = Alphabet $ ['0'..'9'] ++ ['a'..'z'] ++ ['A'..'Z']
base64 = Alphabet $ ['A'..'Z'] ++ ['a'..'z'] ++ ['0'..'9'] ++ "+/"

convToInt :: Alphabet -> String -> Integer
convToInt fmt xs = sum $ zipWith (*) (map (\x -> bas ^ (toInteger x)) [0..])
  (map (\x -> toInteger . fromJust $ elemIndex x dgt) $ reverse xs)
  where dgt = getDigits fmt
        bas = toInteger $ length dgt

convFromInt :: Alphabet -> Integer -> String
convFromInt fmt n
  | n >= bas = convFromInt fmt (n `quot` bas) ++ res
  | otherwise = res
  where dgt = getDigits fmt
        bas = toInteger $ length dgt
        res = [dgt !! (fromIntegral (n `mod` bas))]

convert :: Alphabet -> Alphabet -> String -> String
convert src dst xs = convFromInt dst (convToInt src xs)
