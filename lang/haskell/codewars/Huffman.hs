module Huffman
    (frequencies
      , encode
      , decode
      , Bit(..)
    ) where


import Data.Function (on)
import Data.List (sort, group, insertBy, sortBy, nub)
import Data.Tuple (swap)
import Control.Arrow ((&&&))
import qualified Data.Map as Map (fromList, Map, (!), lookup)
import Data.Maybe (fromJust)

data Bit = Z | O deriving (Show, Eq)

frequencies :: Ord a => [a] -> [(a, Int)]
frequencies = map (head &&& length) . group . sort

data Tree n v = Leaf n v
              | Node n (Tree n v) (Tree n v)
            deriving (Show, Read, Eq)

getFreq :: Tree Int v -> Int
getFreq (Leaf a _) = a
getFreq (Node a _ _) = a

-- must compare with freq instead of raw inheriting Order
cmpTree = compare `on` getFreq

cmbTree a b = Node (getFreq a + getFreq b) a b

buildHuff [] = error "Huffman: empty list"
buildHuff [x] = x
buildHuff (x:y:xs) = buildHuff (insertBy cmpTree (cmbTree x y) xs)

freq2Tree = buildHuff . sortBy cmpTree . map (uncurry Leaf . swap)

insertBit b (a, bs) = (a, b:bs)

tree2Dct (Leaf _ v) = [(v, [])]
tree2Dct (Node _ left right) = (map (insertBit Z) $ tree2Dct left) ++
    (map (insertBit O) $ tree2Dct right)

freq2Dct :: Ord k => [(k, Int)] -> Map.Map k [Bit]
freq2Dct = Map.fromList . tree2Dct . freq2Tree

encode :: Ord a => [(a, Int)] -> [a] -> Maybe [Bit]
encode freq = fmap concat . mapM (flip Map.lookup d)
    where d = freq2Dct freq

decodeTree r (Node _ left _) (Z:bs) = decodeTree r left bs
decodeTree r (Node _ _ right) (O:bs) = decodeTree r right bs
decodeTree r (Leaf _ v) [] = Just [v]
decodeTree r (Leaf _ v) bs = fmap (v:) $ decodeTree r r bs
decodeTree r _ [] = Nothing

decode :: [(a, Int)] -> [Bit] -> Maybe [a]
decode freq bs = decodeTree t t bs
    where t = freq2Tree freq

norm xs = map (flip (/) s) xs
    where s = sum xs

h x = -1 * (logBase 2 x)

entropy xs = sum $ zipWith (*) ys $ map h ys
    where ys = norm xs

upperIdx2 n = ceiling $ logBase 2 $ until ((<=) (fromIntegral n)) ((*) 2) 1

compressRatio freq cs = ((/) `on` (toRational))
    (length $ fromJust $ encode freq cs)
    ((length cs) * (upperIdx2 $ length $ nub cs))

str = "aaaabcc"

cs = "abc"
bs = encode (frequencies str) cs
cs1 = "abcd"
bs1 = encode (frequencies str) cs1
cs_ = decode (frequencies str) $ fromJust bs
cs2 = decode (frequencies str) [Z]

verify freq cs = ((==) cs) $ fromJust $ decode freq $ fromJust $ encode freq cs
