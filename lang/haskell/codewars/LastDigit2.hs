module LastDigit where

-- [1..y]
upmod 0 _ = 0
upmod x y = (x - 1) `mod` y + 1

lastDigit :: Integer -> Integer -> Integer
lastDigit a b = ((a `mod` 10) ^ (b `upmod` 4 )) `mod` 10
