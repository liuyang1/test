import Data.List (group, sortOn)
import Data.Function (on)
import Control.Arrow ((&&&))

f = concatMap (uncurry replicate) . sortOn (negate . fst) . map (length &&& head) . group

main = print $ f [1, 1, 2, 2, 3, 4, 4, 4, 4]
-- result: [4, 4, 4, 4, 1, 1, 2, 2, 3]
