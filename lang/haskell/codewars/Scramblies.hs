module Scramblies where

import Data.List
import Control.Arrow
import qualified Data.Map as Map
import Data.Function

count :: String -> Map.Map Char Int
count = Map.fromList . map (head &&& length) . group . sort

cmp xs ys = compare `on` count

scramble xs ys = EQ == (cmp xs ys)
