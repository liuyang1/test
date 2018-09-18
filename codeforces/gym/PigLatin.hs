module PigLatin where

pigLatin = unwords . map conv . words
    where conv (x:xs) = xs ++ [x] ++ "ay"
