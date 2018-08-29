import Data.Function (on)
import Data.List (scanl1, tails, inits)

-- divide & conquer algo
accum = scanl1 (+)

maxCross xs low mid high = ((+) `on` (maximum . accum)) ml r - midv
    where sub xs low high = take (high - low + 1) $ drop low xs
          ml = reverse $ sub xs low mid
          r = sub xs mid high
          midv = xs !! mid

maxSub xs low high
  | low == high = xs !! mid
  | otherwise = maximum [maxSub xs low mid
                        ,maxSub xs (mid + 1) high
                        ,maxCross xs low mid high]
                            where mid = (low + high) `div` 2

maxSubarray0 xs = maxSub xs 0 (length xs - 1)

-- one line smart Kadane's algo
-- ref: https://en.wikipedia.org/wiki/Maximum_subarray_problem
maxSubarray1 xs = maximum $ scanl1 (\m i -> max i (i + m)) xs

-- naive method
maxSubarray2 = maximum . map (maximum . accum) . init . tails

-- test code
a0 = [-2, 1, -3, 4, -1, 2, 1, -5, 4]
a1 = [1]
a2 = [-1]
a3 = [1, 2]

test f = map (\x -> f x == maxSubarray1 x) [a0, a1, a2, a3]

main = do
    print $ test maxSubarray0
    print $ test maxSubarray2
