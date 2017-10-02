module SmartCase where

import Data.Char

-- native match function
match _ _ "" _ = True
match _ _ _ "" = False
match cmp xs (x:xt) yy@(y:yt)
    | cmp x y = match cmp xs xt yt
    | otherwise = match cmp xs xs yt

sensitive xs ys = match (==) xs xs ys

incmp x y = (toUpper x) == (toUpper y)

insensitive xs ys = match incmp xs xs ys

smartCase xs ys
    | any isUpper xs = sensitive xs ys
    | otherwise = insensitive xs ys

main = do
        print $ False == sensitive "abc" "def"
        print $ True == sensitive "abc" "  abc  "
        print $ True == sensitive "abc" "  ab abc  "
        print $ False == sensitive "abc" "  ab ABC "
        print $ True == smartCase "abc" "  ab ABC "
        print $ False == smartCase "Abc"  "  ab ABC "
