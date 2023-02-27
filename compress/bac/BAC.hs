-- p0=p, p1=1-p0, xs is sequence of [0,1]
en0 _ [] (b,e) = (b,e)
en0 p (x:xs) (b,e) = en0 p xs (if x == 0 then (b,m) else (m,e))
    where m = b+(e-b)*p

rescale (xs,b,e)
    | e < 0.5 = (0:xs,2*b,2*e)
    | b > 0.5 = (1:xs,2*b-1,2*e-1)
    | otherwise = (xs,b,e)

{-
en1 p (b,e) [] = []
en1 p (b,e) (x:xs) = en0 p xs (if x == 0 then (b,m) else (m,e))
    where m = b+(e-b)*p
-}
main = do
    print $ en0 0.1 [1] (0,1)
    print $ en0 0.1 [0] (0,1)
    print $ en0 0.1 [1,1,0,1] (0,1)
