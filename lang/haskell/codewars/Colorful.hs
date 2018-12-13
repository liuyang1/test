
data Color = R | G | B deriving (Eq, Show, Read)

rule R G = B
rule G B = R
rule B R = G
rule a b
    | a == b = a
    | otherwise = rule b a

f [] = []
f [x] = [x]
f [x, y] = [rule x y]
f (x:y:xs) = (rule x y):(f (y:xs))

go [] = []
go [x] = [x]
go xs = go $ f xs

seq0 = [R, R, G, B, G, G, G, G, G, G, G, G]

main = do
    mapM_ print $ take 15 $ iterate f seq0
