module TestList where

import Data.List

-- (\\) remove first occurence of X in Lst
testSlash = do
        print $ [1, 2, 3, 5] == [1..5] \\ [4]
        print $ [1, 2, 3, 5] ++ [1..5]== ([1..5] ++ [1..5]) \\ [4]
