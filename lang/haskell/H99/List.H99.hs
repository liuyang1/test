-- ref: https://wiki.haskell.org/99_questions/1_to_10
module List where

import System.Random

myLast [] = undefined
myLast [x] = x
myLast (x:xs) = myLast xs

myButLast [] = undefined
myButLast [x] = undefined
myButLast [x,_] = x
myButLast (x:xs) = myButLast xs

elementAt 0 _ = undefined
elementAt 1 (x:_) = x
elementAt n (_:xs) = elementAt (n - 1) xs

elementAt n xs
  | n < 1 = undefined
  | n == 1 = head xs
  | otherwise = elementAt (n - 1) (tail xs)

myLength [] = 0
myLength (_:xs) = 1 + myLength xs

myReverse [] = []
myReverse (x:xs) = myReverse xs ++ [x]

isPalindrome xs = xs == myReverse xs

data NestedList a = Elem a | List [NestedList a]

flatten (Elem x) = [x]
flatten (List xs) = concatMap flatten xs

-- It like group in Data.List
pack [] = []
pack (x:xs) = go [x] xs
    where go ys [] = [ys]
          go ys@(y:_) (x:xs)
            | y == x = go (x:ys) xs
            | otherwise = ys: (go [x] xs)

rlencode [] = []
rlencode (x:xs) = go 1 x xs
    where go n x [] = [(n, x)]
          go n x yys@(y:ys)
              | y == x = go (n + 1) x ys
              | otherwise = (n, x): rlencode yys

-- ref: https://wiki.haskell.org/99_questions/11_to_20
data Compress a = Single a
                | Multiple Int a
              deriving (Show)

build 1 a = Single a
build n a = Multiple n a

encodeModified [] = []
encodeModified (x:xs) = go 1 x xs
    where go n x [] = [build n x]
          go n x yys@(y:ys)
              | y == x = go (n + 1) x ys
              | otherwise = (build n x): encodeModified yys

unpack (Single a) = [a]
unpack (Multiple n a) = replicate n a

decodeModified xs = concatMap unpack xs

-- skip Problem 13, as we already get direct solution

dupli [] = []
dupli (x:xs) = x: x: dupli xs

repli xs n = concatMap (replicate n) xs

-- index from 1
dropEvery xs n = map fst $ filter (\(_,i) -> i `mod` n /= 0) $ zip xs [1..]

mySplit xs 0 = ([], xs)
mySplit (x:xs) n = (x:h, t)
    where (h, t) = mySplit xs (n - 1)

slice xs begin end = ret
    where (_, mid) = mySplit xs (begin - 1)
          (ret, _) = mySplit mid (end - begin)

rotate xs n = reverse $ reverse h ++ reverse t
    where (h, t) = mySplit xs (mod n $ length xs)

removeAt 1 (x:xs) = (x,xs)
removeAt n (_:xs) = removeAt (n - 1) xs

-- ref: https://wiki.haskell.org/99_questions/21_to_28
insertAt x ys 1 = x:ys
insertAt x (y:ys) n = y: (insertAt x ys) (n - 1)

range begin end
  | begin > end = []
  | otherwise = begin: range (begin + 1) end

-- rndSelect xs n
g = newStdGen
go g = fmap (take 10 . randomRs ('a', 'z')) g

rndSelect xs n = (fmap (map (xs !!) . take n . randomRs (0, (length xs) - 1)) g)

-- diffSelect 0 m = return []
-- diffSelect n m = rndSelect [1..m] 1
    -- where
-- go xs 0 = return []
-- go xs n = go xs (n - 1) >>= ((rndSelect xs 1) :)
    -- return x0 ++ (diffSelect (n - 1) m)

-- 26, combination
combinations n xxs@(x:xs)
  | n == 1 = map (: []) xs
  | n == length xxs = [xxs]
  | otherwise = map (x :) (combinations (n - 1) xs) ++ combinations n xs
