swap0 (a, b) = (b, a)

same x = uncurry (,) x

swap1 = uncurry (flip (,))

swap2 = ((flip (,)) `uncurry`)

-- uncurry it, then flip arguments, then combine them to pair
-- :)
swap3 = (((,) `flip`) `uncurry`)

main = do
        print $ swap0 (1, 2)
        print $ same (1, 2)
        print $ swap1 (1, 2)
        print $ swap2 (1, 2)
        print $ swap3 (1, 2)
