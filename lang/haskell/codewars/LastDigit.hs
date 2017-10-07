module LastDigit where

-- [1..y]
upmod 0 _ = 0
upmod x y = (x - 1) `mod` y + 1

lastDigit2 :: Integer -> Integer -> Integer
lastDigit2 _ 0 = 1
lastDigit2 0 _ = 0
lastDigit2 1 _ = 1
lastDigit2 2 y = case y `upmod` 4 of
                    1 -> if y == 1 then 2 else 12
                    2 -> 4
                    3 -> 8
                    4 -> 16
lastDigit2 3 y = case y `upmod` 4 of
                   1 -> 3
                   2 -> 9
                   3 -> 7
                   4 -> 1
lastDigit2 4 y = case y `upmod` 2 of
                   1 -> 4
                   2 -> 16
lastDigit2 5 y = 5
lastDigit2 6 y = if y == 1 then 6 else 16
lastDigit2 7 y = case y `upmod` 4 of
                   1 -> 7
                   2 -> 9
                   3 -> 3
                   4 -> 1
lastDigit2 8 y = case y `upmod` 4 of
                   1 -> 8
                   2 -> 4
                   3 -> 12
                   4 -> 16
lastDigit2 9 y = case y `upmod` 2 of
                   1 -> 9
                   2 -> 1
lastDigit2 10 y = if y == 1 then 10 else 20
lastDigit2 11 y = [1, 11, 1] !! (fromIntegral (y `upmod` 2))
lastDigit2 12 y = [1, 12, 4, 8, 16] !! (fromIntegral (y `upmod` 4))
lastDigit2 13 y = [1, 13, 9, 17, 1] !! (fromIntegral (y `upmod` 4))
lastDigit2 14 y = if y == 1 then 14 else [16, 4, 16] !! (fromIntegral (y `upmod` 2))
lastDigit2 15 y = [1, 15, 5] !! (fromIntegral (y `upmod` 2))
lastDigit2 16 y = 16
lastDigit2 17 y = [1, 17, 9, 13, 1] !! (fromIntegral (y `upmod` 4))
lastDigit2 18 y = if y == 1 then 18 else [16, 8, 4, 12, 16] !! (fromIntegral (y `upmod` 4))
lastDigit2 19 y = [1, 19, 1] !! (fromIntegral (y `upmod` 2))
lastDigit2 20 y = 20
lastDigit2 x y = lastDigit2 (x `upmod` 20) y

hlp :: [Integer] -> Integer
hlp [] = 1
hlp [x] = x
hlp (x:xs) = lastDigit2 x (hlp xs)

lastDigit xs = (hlp xs) `mod` 10
