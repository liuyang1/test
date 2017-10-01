-- flip function, f x y -> f y x
-- pointless style
-- same with
-- reverse xs = foldl (flip (:)) [] xs
reverse = foldl (flip (:)) []

main = print $ reverse [1..10]
