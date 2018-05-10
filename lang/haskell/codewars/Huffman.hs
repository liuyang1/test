module Huffman
    (frequencies
      , encode
      , decode
      , Bit(..)
    ) where


import Data.Function
import Data.Monoid
import Data.List (sort, group, insert, insertBy, sortBy)
import Data.Tuple (swap)
import Control.Arrow ((&&&))
import Data.Map (fromList, Map, (!))
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

cmpTree = (compare `on` getFreq)

cmbTree a b = Node (getFreq a + getFreq b) a b

buildHuff [] = error "Huffman: empty list"
buildHuff [x] = x
buildHuff (x:y:xs) = buildHuff (insertBy cmpTree (cmbTree x y) xs)

freq2Tree = buildHuff . sortBy cmpTree . map (uncurry Leaf . swap)

insertBit b (a, bs) = (a, b:bs)

tree2Dct (Leaf _ v) = [(v, [])]
tree2Dct (Node _ left right) = (map (insertBit Z) $ tree2Dct left) ++
    (map (insertBit O) $ tree2Dct right)

freq2Dct :: Ord k => [(k, Int)] -> Map k [Bit]
freq2Dct = fromList . tree2Dct . freq2Tree

encode :: Ord a => [(a, Int)] -> [a] -> Maybe [Bit]
encode freq cs = Just $ concatMap (d !) cs
    where d = freq2Dct freq

-- when CS contain invalid char, it should return Nothing

decodeTree r (Node _ left _) (Z:bs) = decodeTree r left bs
decodeTree r (Node _ _ right) (O:bs) = decodeTree r right bs
decodeTree r (Leaf _ v) bs = v: decodeTree r r bs
decodeTree r _ [] = []
-- decodeTree r _ _ = Nothing
decode :: [(a, Int)] -> [Bit] -> Maybe [a]
decode freq bs = Just $ decodeTree t t bs
    where t = freq2Tree freq

str = "aaaabcc"

cs = "abc"
bs = encode (frequencies str) cs
cs_ = decode (frequencies str) $ fromJust bs

verify freq cs = ((==) cs) $ fromJust $ decode freq $ fromJust $ encode freq cs
