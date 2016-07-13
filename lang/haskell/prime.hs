union (x:xs) (y:ys) = case (compare x y) of
                        LT -> x : union xs (y:ys)
                        EQ -> x : union xs ys
                        GT -> y : union (x:xs) ys
union xs ys = xs ++ ys

-- fixpoint combinator
_Y :: (t -> t) -> t
_Y g = g (_Y g)



primesTME = 2: _Y ((3:) . gaps 5 . joinT . map (\p -> [p*p, p*p+2*p..]))

joinT ((x:xs):t) = x: union xs (joinT (pairs t))
    where pairs (xs:ys:t) = union xs ys: pairs t

gaps k s@(x:xs) | k < x = k: gaps (k + 2) s
  | otherwise = gaps (k + 2) xs
