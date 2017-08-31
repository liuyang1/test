module EmulationPreventionThreeByte where

data St = Zro
        | One
        | Two

flt _ [] = []
flt Zro (0:xs) = 0: flt One xs
flt One (0:xs) = 0: flt Two xs
flt Two (3:xs) = flt Zro xs
flt _ (x:xs) = x: flt Zro xs

preventThree = flt Zro

main = print . preventThree $ [3, 0, 0, 3, 10, 0, 0, 2, 0, 1, 3]
