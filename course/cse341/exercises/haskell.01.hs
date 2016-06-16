{- 01
"squid" ++ "clam" -- String
[True, False, True, True] -- [Bool]
[True, False, 'a'] -- error
(True, False, 'a') -- (Bool, Bool, Char)
-}

cube :: Double -> Double
cube x = x * x * x

sum :: Double -> Double -> Double -> Double
sum x y z = x + y + z

quad :: Double -> Double -> Double -> Double -> Double
quad a b c x =
    a * x * x + b * x + c

rev :: [a] -> [a]
rev [] = []
rev (x:xs) = rev xs ++ [x]

myMap2 :: (a -> b -> t) -> [a] -> [b] -> [t]
myMap2 f xs ys = map (uncurry f) $ zip xs ys
-- myMap2 = zipWith

doubles = 10: map (* 2) doubles

doubles1 = iterate (* 2) 10

dollars = iterate (* 1.05) 100

myConst :: t1 -> t -> t1
myConst c x = c

-- myConst True :: t -> Bool

append :: [a] -> [a] -> [a]
append [] ys = ys
append (x:xs) ys = x: append xs ys

-- append [] :: [a] -> [a]
-- append [True] :: [Bool] -> [Bool]

myMap :: (t -> a) -> [t] -> [a]
myMap f [] = []
myMap f (x: xs) = f x: myMap f xs

-- myMap (myConst True) :: [t] -> [Bool]

-- myConst 5 "octopus" => 5
-- myMap (myConst "squid") [1 ..] => ["squid", "squid", ..]
-- myMap sqrt [1, 2, 100] => [1, 1.414, 10]
