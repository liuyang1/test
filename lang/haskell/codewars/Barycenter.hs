module Codewars.G964.Barycenter where

myround n =  (/ (10.0^n)) . fromInteger . round . (* (10^n))

barTriang :: (Double, Double) -> (Double, Double) -> (Double, Double) -> (Double, Double)
barTriang (a, b) (c, d) (e, f) = (fn a c e, fn b d f)
    where fn a b c = myround 4 $ (a + b + c) / 3
