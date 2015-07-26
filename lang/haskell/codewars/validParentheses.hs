module Codewars.Parentheses where

validParentheses :: String -> Bool
validParentheses xs = helper xs 0
  where helper [] acc = acc == 0
        helper ('(':xs) acc = helper xs (acc + 1)
        helper (')':xs) acc = acc /= 0 && helper xs (acc - 1)
