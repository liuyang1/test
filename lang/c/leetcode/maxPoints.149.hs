newtype Point = Pt x y deriving (Show, Ord)
newtype Seg = TwoPt a b c deriving (Show, Ord)

gcd 0 b = b
gcd a 0 = a
gcd a b
  | a == b = a
  | even a && even b = 2 * gcd (a `div` 2) (b `div` 2)
  | even a = gcd (a `div` 2) b
  | even b = gcd a (b `div` 2)
  | otherwise = if a > b then gcd (a `mod` b) b else gcd (b `mod` a) a

sig3 0 0 c = signum c
sig3 0 b _ = signum b
sig3 a _ _ = signum a

abs x = if x >= 0 then x else (-x)

gcd3 a b c = (gcd (gcd (abs a) (abs b)) (abs c)) * (sig3 a b c)
