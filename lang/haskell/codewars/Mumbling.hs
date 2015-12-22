module Codewars.G964.Accumule where
import Data.Char
import Data.List

accum :: String -> String
accum s = intercalate "-" $ zipWith f (map toLower s) [0..]
  where f c n = (toUpper c) : (replicate n c)
