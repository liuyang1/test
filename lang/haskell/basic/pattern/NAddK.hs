module NAddK where

-- This is inefficent way. Just for demo.
isEven 0 = True
isEven 1 = False
isEven n = case compare n 0 of
               LT -> isEven (n + 2)
               GT -> isEven (n - 2)
-- It don't work
-- isEven (n + 2) = isEven n
-- >>> Haskell 2010 不再支持这种形式的模式匹配

main = do
        print $ False == isEven 3
        print $ True == isEven 8
        print $ False == isEven (-3)
        print $ True == isEven (-8)
