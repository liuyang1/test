module CodeWars.Sqrt where
import Prelude hiding (sqrt)

sqrtInth n x
  | xx < n = sqrtInth n (x + 1)
  | xx == n = Right x
  | otherwise = Left (x - 1, x)
    where xx = x * x

sqrtInt :: Integral n => n -> Either (n, n) n
sqrtInt n = sqrtInth n 0
