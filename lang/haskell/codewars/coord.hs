module Coordinates where
import Control.Monad
import Data.Bifunctor
roundn d n = fromInteger (round $ n * (10 ^ d)) / (10.0 ^^ d)

coordinates :: Double -> Double -> (Double, Double)
coordinates a r = join bimap (roundn 10 . (r *)) (cos theta, sin theta)
  where theta = a / 180 * pi
