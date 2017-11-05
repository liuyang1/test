module MaxSequence where
import Data.List (scanl)

-- Return the greatest subarray sum within the array of integers passed in.
maxSequence :: [Int] -> Int

fn acc x = if acc >= 0 then acc + x else x
maxSequence xs = maximum $ scanl fn 0 xs
