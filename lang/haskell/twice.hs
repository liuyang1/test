applyTwice :: (a -> a) -> a -> a
applyTwice f x = f (f x)

square x = x * x

main = print $ applyTwice square 2
-- 16
