replace a b = map (\x -> if x == a then b else x)
replaces as bs = (foldr (.) id $ zipWith replace as bs)

main = print $ replaces ",1" ".0" "127,1,1,1"
