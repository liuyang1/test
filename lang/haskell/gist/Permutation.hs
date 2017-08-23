module Permutation where

insertEvery :: t -> [t] -> [[t]]
insertEvery x [] = [[x]]
insertEvery x ys@(y: yt) = (x: ys) : map (y :) (insertEvery x yt)

permutation xs = foldr (concatMap . insertEvery) [[]] xs

a = insertEvery <$> [0] <*> [[1, 2, 3]]

b = fmap (insertEvery 0) [[1, 2, 3], [2, 3, 1]]
-- for List monad, `fmap` is equal to `map`

c = map (insertEvery 0) [[1, 2, 3], [2, 3, 1]]

d = 0 `insertEvery` [1, 2, 3]

pperm :: t -> [[t]] -> [[t]]
pperm = concatMap . insertEvery
e = 3 `pperm` (2 `pperm` ( 1 `pperm` [[]]))
-- => foldr style
