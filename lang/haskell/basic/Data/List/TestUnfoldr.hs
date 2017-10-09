import Data.List (unfoldr)
-- ref: http://hackage.haskell.org/package/base-4.10.0.0/docs/Data-List.html#v:unfoldr

-- unfoldr :: (b -> Maybe (a, b)) -> b -> [a]

-- The unfoldr function is a `dual' to foldr:
-- while foldr reduces a list to a summary value,
--      unfoldr builds a list from a seed value.
-- The function takes the element and returns Nothing if it is done producing the list,
--      or returns Just (a,b), in which case, a is a prepended to the list and b is used as the next element in a recursive call.

myIterate f = unfoldr (\x -> Just (x, f x))
a = take 10 $ myIterate (*2) 1

-- In some cases, unfoldr can undo a foldr operation:

-- unfoldr f' (foldr f z xs) == xs
-- if the following holds:

-- f' (f x y) = Just (x,y)
-- f' z       = Nothing

-- A simple use of unfoldr:

b = unfoldr (\b -> if b == 0 then Nothing else Just (b, b-1)) 10
--  [10,9,8,7,6,5,4,3,2,1]

-- ref: https://stackoverflow.com/questions/12876384/grouping-a-list-into-lists-of-n-elements-in-haskell
chunkOf n = takeWhile (not.null) . unfoldr (Just . splitAt n)

c = chunkOf 3 ['A'..'Z']
