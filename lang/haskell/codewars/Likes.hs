module Likes where

likes :: [String] -> String
likes [] = likes ["no one"]
likes (a:[]) = a ++ " likes this"
likes (a:b:[]) = a ++ " and " ++ b ++ " like this"
likes (a:b:c:[]) = a ++ ", " ++ likes [b,c]
likes (a:b:xs) = likes [a, b, (show $ length xs) ++ " others"]
