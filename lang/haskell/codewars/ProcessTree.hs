module ProcessTree where

import Data.Function (on)
import Data.List (sort, groupBy)

type PID = Int
data Process = Process PID [Process] deriving (Show)

-- I prerfer (parent, child) struct
swap (a, b) = (b, a)

singleton :: PID -> Process
singleton x = Process x []

buildChild :: [(PID, PID)] -> Process
buildChild xs = Process root childs
    where root = fst $ head xs
          childs = map (singleton . snd) xs

merge :: Process -> Process -> Process
merge (Process r0 ps0) pid@(Process r1 ps1)
    | r0 == r1 = Process r0 (ps0 ++ ps1)
    | otherwise = Process r0 (map (`merge` pid) ps0)

fstChild :: Process -> Process
fstChild (Process _ (p:_)) = p

makeTree :: [(PID, PID)] -> Process
makeTree = fstChild . foldl1 merge . map buildChild . groupBy ((==) `on` fst ) . sort . map swap

-- :) smart way, but maybe slow,
-- I am not sure about this point. but it traverl on xs too many times
makeTree1 xs = head $ kids (-1)
    where kids p = [Process pid (kids pid) | (pid, ppid) <- xs, ppid == p]

makeTree2 = fstChild . go (-1)
    where go pid xs = Process pid [go child xs | (child, ppid) <- xs, ppid == pid]

main = do
        print $ makeTree xs
        print $ makeTree1 xs
        print $ makeTree2 xs
        where xs = [(1, -1), (219, 214), (214, 1), (124,1)]
