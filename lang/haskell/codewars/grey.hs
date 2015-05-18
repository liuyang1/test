module Codewars.Kata.Grey where

num2hex n = let lst = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f']
            in lst !! n

num2hex2 n = if n > 255 || n < 0 then [] else [num2hex (n `div` 16), num2hex (rem n 16)]

num2grey n = "#" ++ (concat $ replicate 3 (num2hex2 n))

shadesOfGrey :: Int -> [String]
shadesOfGrey n = if n <= 0 then []
                           else if n > 254 then shadesOfGrey 254
                           else map num2grey [1..n]
