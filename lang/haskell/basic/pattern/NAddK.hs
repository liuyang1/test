module NAddK where

-- This is inefficent way. Just for demo.
isEven 0 = True
isEven 1 = False
isEven n = case compare n 0 of
               LT -> isEven (n + 2)
               GT -> isEven (n - 2)
-- It don't work
-- isEven (n + 2) = isEven n

main = do
        print $ False == isEven 3
        print $ True == isEven 8
        print $ False == isEven (-3)
        print $ True == isEven (-8)
