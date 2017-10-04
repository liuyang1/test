import Data.Char (toUpper)
import Data.List (intersperse)

-- 建议始终加上函数的类型，便于阅读。毕竟程序是写给程序员的，而不是机器的。
song :: Int -> String
song n = concat $ intersperse "\n" $ map verse [1..n]

capital :: String -> String
capital (x:xs) = (toUpper x): xs

verse :: Int -> String
verse n = concatMap ((++ "\n") . capital . ($ n)) [line1, line2, line3, line4]

showNum :: Int -> String
showNum n = ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"] !! n

plural :: Int -> String
plural n = showNum n ++ man n
    where man 1 = " man"
          man _ = " men"

line1 :: Int -> String
line1 n = (plural n) ++ " went to mow"

line2 :: Int -> String
line2 _ = "went to mow a meadow"

line3 :: Int -> String
line3 n = (concat $ intersperse ", " $ map plural [n, n - 1.. 1]) ++ " and his dog"

line4 :: Int -> String
line4 = line2

main = putStrLn $ song 5
