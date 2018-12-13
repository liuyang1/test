myAnd True True = True
myAnd _ _ = False

myOr False False = False
myOr _ _ = True

myNot = not

(.:) f g a b = f $ g a b

myNand = myNot .: myAnd

myNor = myNot .: myOr

myXor a b = a /= b

-- prod2 xs ys = concatMap (\x-> zip (repeat x) ys) xs
prod2 xs ys = (,) <$> xs <*> ys

table f2 = (\x y -> (x, y, f2 x y)) <$> t <*> t
    where t = [True, False]

test a b = myAnd a (myOr a b)
test' a b = a `myAnd` (a `myOr` b)

prodn 1 xs = map (:[]) xs
prodn n xs = concatMap (\x -> map (\y -> x:y) ys) xs
    where ys = prodn (n - 1) xs

tableN n f = map f $ prodn n t
    where t = [True, False]

gray 1 = ["0", "1"]
gray n = map (\x -> "0"++x) gn ++ map (\x -> "1" ++ x) (reverse gn)
    where gn = gray (n - 1)
