import Control.Arrow ((&&&))
import Data.List (group, sort)
import Data.Ratio ((%))

comb xxs@(x:xs) n
  | n == length xxs = [xxs]
  | n == 1 = map (: []) xxs
  | otherwise = map (x :) (comb xs (n - 1)) ++ comb xs n

comba xs = concatMap (comb xs) [1..(length xs)]

subOne [] = []
subOne (x:xs) = xs : map (x: ) (subOne xs)

filterBy _ [] = []
filterBy (True:bs) (x:xs) = x: filterBy bs xs
filterBy (False:bs) (_:xs) = filterBy bs xs

freq xs = map (head &&& (\x-> length x % n)) $ group $ sort xs
    where n = length xs

banzaf xs maj = map (\((_, b), c) -> (b, c)) $ freq $ concatMap key $ cand $ zip [1..] xs
    where cand xs = filter isMaj (comba xs)
          key xs = filterBy (map (not . isMaj) (subOne xs)) xs
          isMaj xs = sum (map snd xs) > maj
