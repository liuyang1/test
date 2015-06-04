module HughesList where
import Data.Monoid

newtype Hughes a = Hughes ([a] -> [a])

runHughes :: Hughes a -> [a]
runHughes (Hughes k) = k []

mkHughes :: [a] -> Hughes a
mkHughes x = Hughes (x ++)

consDumb :: a -> Hughes a -> Hughes a
consDumb a h = mkHughes (a : runHughes h)

cons :: a -> Hughes a -> Hughes a
cons a (Hughes b) = Hughes ((a:) . b)

appendDumb :: Hughes a -> Hughes a -> Hughes a
appendDumb a b = mkHughes (runHughes a ++ runHughes b)

append :: Hughes a -> Hughes a -> Hughes a
append (Hughes a) (Hughes b) = Hughes (a . b)

instance Monoid (Hughes a) where
  mempty = mkHughes []
  mappend = append

snocDumb :: Hughes a -> a -> Hughes a
snocDumb l a = mkHughes (runHughes l ++ [a])

snoc :: Hughes a -> a -> Hughes a
snoc (Hughes l) a = Hughes (l . (a:))
