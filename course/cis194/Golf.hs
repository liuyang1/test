--------------------------------------------------------------------------------
-- Homework 03
import Data.List
import Control.Arrow

--------------------------------------------------------------------------------
-- skips
-- generate list of list, which skip 0 element, 1 element, ...

-- take element every N
every n xs = case drop (n - 1) xs of
               (y: ys) -> y: every n ys
               [] -> []
skips xs = map (`every` xs) [1..(length xs)]

--------------------------------------------------------------------------------
-- local maximium
-- generate triple tuple, then filter on it.
localMaxima xs = map (\(_, y, _) -> y) $ filter (\(x, y, z) -> y > x && y > z) ys
    where ys = zip3 (drop 2 xs) (drop 1 xs) xs

--------------------------------------------------------------------------------
-- histogram
-- Input: list of integer, between 0 and 9
-- Output: vertical histogram showing freqeunce of number in list

-- statistic on list, get [(number, count)] list
stat xs = map (head &&& length) . group . sort $ xs

-- genereate to one horizontal line
plotLine xs = map (\x -> if x `elem` ks then '*' else ' ') [0..9]
    where ks = map fst xs

-- decreate count for every number
decHist xs = filter (\(k, v) -> v /= 0) $ map (\(k, v) -> (k, v - 1)) xs
-- iterate on decHist func, and concat to a list
decEnd [] = []
decEnd xs = xs: decEnd (decHist xs)
-- plot vertical histogram
plot = reverse . map plotLine . decEnd

-- add axis line, and label number
axis xs = xs ++ [replicate 10 '=', concatMap show [0..9]]

histogram :: [Integer] -> String
histogram = unlines . axis . plot . stat
