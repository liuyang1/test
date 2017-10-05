-- 打印九九乘法表
import Data.List (intercalate)

tbl = unlines $ map (unwords . (map showUnit)) xs
    where showUnit (a, b) = show a ++ "*" ++ show b ++ "=" ++ show (a * b)
          xs = [[(a, b)| a <- [1..b]] | b <- [1..9]]

main = putStrLn tbl
