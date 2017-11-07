module Codewars.Kata.Braces where

lefts = ['{',  '[',  '(']
rights = ['}', ']', ')']
pair '{' '}' = True
pair '[' ']' = True
pair '(' ')' = True
pair _ _ = False

hlp [] [] = True
hlp [] _ = False
hlp (x:xs) ys
  | x `elem` lefts = hlp xs (x:ys)
  | x `elem` rights = (not (null ys)) && pair (head ys) x && hlp xs (tail ys)
  | otherwise = False

validBraces :: String -> Bool
validBraces xs = hlp xs []

-- better solution
validBraces_ = null . foldr op []
    where op '{' ('}': xs) = xs
          op '[' (']': xs) = xs
          op '(' (')': xs) = xs
          op x xs = x: xs

test f = do
    print $ f "(){}[]"
    print $ f "([{}])"
    print $ f "(}"
    print $ f "[(])"
    print $ f "[({})](])])"
    print $ f "()"
    print $ f "[([)"
    print $ f "())({}}{()][]["
    print $ f "({})[({})]"

main = test validBraces_
