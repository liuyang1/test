pi0 = toRational pi

pi1 = 22 `divMod` 7
pi2 = 22 `div` 7
pi3 = 22 `quot` 7

main = do
        print pi0
        print pi1
        print pi2
        print pi3
