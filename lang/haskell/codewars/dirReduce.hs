module Codewars.Kata.Reduction where
import Codewars.Kata.Reduction.Direction
-- data Direction = North | East | West | South deriving (Eq, Show)

dirf [] x = [x]
dirf (South:xs) North = xs
dirf (North:xs) South = xs
dirf (West:xs) East = xs
dirf (East:xs) West = xs
dirf xs x = x:xs

dirReduce :: [Direction] -> [Direction]
dirReduce = reverse . foldl dirf []
