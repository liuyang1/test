import Data.Char (toUpper)
import Data.List (intercalate)

-- 建议始终加上函数的类型，便于阅读。毕竟程序是写给程序员的，而不是机器的。
song :: Int -> String
song n = intercalate "\n\n" $ map verse [1..n]

capital :: String -> String
capital (x:xs) = (toUpper x): xs

verse :: Int -> String
verse n = intercalate "\n" $ map (capital . ($ n)) [line1, line2, line3, line4]
-- unlines = intercalate "\n"

showNum :: Int -> String
showNum n = ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"] !! n

plural :: Int -> String
plural n = showNum n ++ man n
    where man 1 = " man"
          man _ = " men"

-- 函数标记可以写在一行里面
line1, line2, line3, line4 :: Int -> String

line1 n = (plural n) ++ " went to mow"

line2 _ = "went to mow a meadow"

line3 n = (intercalate ", " $ map plural [n, n - 1.. 1]) ++ " and his dog"

line4 = line2

main = putStrLn $ song 5
