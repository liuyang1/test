module Operator where

import Data.Char

trim1 = reverse . dropWhile isSpace
trim = trim1 . trim1

firstName ## lastName = (trim lastName) ++ ", " ++ (trim firstName)

main = do
        print $ "Yang, Liu" == " Liu " ## " Yang "
        print $ "Word, Hello" == " Hello    " ## "   Word "
