square x = x * x

applyTwice :: (a -> a) -> a -> a
applyTwice f x = f (f x)

applys n f = foldr (.) id $ replicate n f

iter n f x = take n $ iterate f x

main = do
        print $ 16 == applyTwice square 2
        print $ 256 == applys 3 square 2
        print $ [2, 4, 16, 256] == iter 4 square 2
