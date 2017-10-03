module Codewars.Kata.Grey where

num2hex n = let lst = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f']
            in lst !! n

num2hex2 n = if n > 255 || n < 0 then [] else [num2hex (n `div` 16), num2hex (rem n 16)]

num2grey n = "#" ++ (concat $ replicate 3 (num2hex2 n))

shadesOfGrey :: Int -> [String]
shadesOfGrey n
  | n <= 0 = []
  | n > 254 = shadesOfGrey 254
  | otherwise = map num2grey [1..n]
