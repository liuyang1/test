module Fibonacci where
--------------------------------------------------------------------------------
--  Fibonacci
--  matrix multiplication method
--  complexity: log²n
--  # negative
--  1 0 -1 -2 -3 -4 -5 -6
--  1 0  1 -1  2 -3  5 -8
vecProd (a0, a1) (b0, b1) = a0 * b0 + a1 * b1
matProd (a00, a01, a10, a11) (b00, b01, b10, b11)
  = (vecProd r0 l0, vecProd r0 l1
    ,vecProd r1 l0, vecProd r1 l1)
  where r0 = (a00, a01)
        r1 = (a10, a11)
        l0 = (b00, b10)
        l1 = (b01, b11)
matPower a = matProd a a

mat1 = (0, 1, 1, 1)
fibIter 1 = mat1
fibIter n
  | even n = matPower aa
  | otherwise = matProd mat1 (matPower aa)
  where a = n `div` 2
        aa = fibIter a
-- This style, only recursive on n is even. so it's fast.
-- fib(n) = M(n/2) * M((n-1)/2)
-- This is too bad, then it is tree recursive,
--      which need more space, and more time.

fib 0 = 0
fib n
  | n > 0 = x
  | n < 0 && odd n = x
  | otherwise = -1 * x
  where (_, x, _, _) = fibIter $ abs n
