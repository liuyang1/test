module LongestCommonSubsequence where

lcs :: String -> String -> String
lcs [] _ = []
lcs _ [] = []
lcs xx@(x:xs) yy@(y:ys)
  | x == y = x: lcs xs ys
  | length a > length b = a
  | otherwise = b
    where a = lcs xx ys
          b = lcs xs yy
