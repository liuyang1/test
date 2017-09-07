module ComposeFuncs where

square x = x * x

f = foldr (.) id [square, (+ 1),(* 100),(+ 2)]
g = foldl (.) id [square, (+ 1),(* 100),(+ 2)]

main = do
    print $ f 1
    print $ g 1
