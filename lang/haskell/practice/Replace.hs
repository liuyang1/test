module Replace where

-- build a function, which replace x to y
replace :: Eq a => a -> a -> ([a] -> [a])
replace x y = map (\i -> if i == x then y else i)

-- build a function, it iterate to replace x in xs to y in ys
-- from right to left
replacesr :: Eq a => [a] -> [a] -> ([a] -> [a])
replacesr xs ys = foldr (.) id $ zipWith replace xs ys

-- build a function, it iterate to replace x in xs to y in ys
-- from left to right
replacesl :: Eq a => [a] -> [a] -> ([a] -> [a])
-- replacesl xs ys = foldr (\f g x -> g (f x)) id $ zipWith replace xs ys
replacesl xs ys = foldr (flip (.)) id $ zipWith replace xs ys

-- flip :: (a -> b -> c) -> b -> a -> c
-- (.) :: (b -> c) -> (a -> b) -> (a -> c)
-- apply flip (.), so ((b -> c) -> (a -> b) -> (a -> c)) -> (a -> b) -> (b -> c) -> (a -> c)
-- flip (.) :: (a -> b) -> (b -> c) -> (a -> c)
-- (flip (.)) f g = \x -> g (f x)

test f = do
        print $ f ",1" ".0" "127,1,1,1"
        print $ f ",." ".0" "127,1,1,1"
        print $ f ".," "0." "127,1,1,1"

main = do
        print "test on replace from right to left"
        test replacesr
        print "test on replace from left to right"
        test replacesl
