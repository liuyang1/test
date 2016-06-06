-- Haskell: how to use ITERATE instead of recursios1
-- Haskell is lazy, so we could TAKE some from unlimited sequence
ffib (a, b) = (b, a + b)

fib = [i | (_, i) <- xs]
  where xs = take 100 $ iterate ffib (1, 1)
