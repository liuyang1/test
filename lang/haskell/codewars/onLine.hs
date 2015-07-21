module PointsOnALine where
import Data.List

type Point = (Rational, Rational)

xcor p = fst p
ycor p = snd p
isOnLineI (a:b:_) = \c -> ((ycor b) - (ycor a)) * ((xcor c) - (xcor a)) == ((ycor c) - (ycor a)) * ((xcor b) - (xcor a))
onLineI xs
  | length xs <= 2 = True
  | otherwise = all isOnLine $ drop 2 xs
    where isOnLine = isOnLineI $ take 2 xs

onLine :: [Point] -> Bool
onLine = onLineI . nub
