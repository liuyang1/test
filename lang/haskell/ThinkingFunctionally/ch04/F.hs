-- 从后构造列表
data List a = Nil | Snoc (List a) a deriving (Show, Read)

myLast (Snoc _ x) = x

myHead (Snoc Nil x) = x
myHead (Snoc xs _) = myHead xs

toList :: [a] -> List a
toList [] = Nil
toList xs = Snoc (toList (init xs)) (last xs)

fromList :: List a -> [a]
fromList Nil = []
fromList (Snoc xs x) = fromList xs ++ [x]

toList1 = hlp . reverse
        where hlp [] = Nil
              hlp (x:xs) = Snoc (hlp xs) x

fromList1 = reverse . hlp
        where hlp Nil = []
              hlp (Snoc xs x) = x: hlp xs

toList2 xs = foldr (\x ys -> Snoc ys x) Nil $ reverse xs
toList3 xs = foldl (\ys x -> Snoc ys x) Nil xs

sample = toList [1..3]
main = do
        print $ sample
        print $ fromList sample
        print $ myHead sample
        print $ myLast sample
        print $ toList1 [1..4]
        print $ fromList1 sample
        print $ toList2 [1..4]
        print $ toList3 [1..4]
