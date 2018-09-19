module QuaterMult where

-- https://en.wikipedia.org/wiki/Multiplication_algorithm#Quarter_square_multiplication

tbl = concat $ zipWith (\a b -> [a, b]) squares triangulars
    where squares = map (\x->x * x) [0..]
          triangulars = map (\x->x *(x + 1)) [0..]

tbl0 = take 20 tbl

mult x y
  | x >= y = tbl0 !! (x + y) - tbl0 !! (x - y)
  | x < y = mult y x

tbl1 = map (\x->map (\y-> (x, y)) [0..x]) [0..9]

fmt (x, y) = show x ++ "*" ++ show y ++ "=" ++ show (mult x y)

tbl2 = map (unwords . map fmt) tbl1

main = mapM_ putStrLn tbl2
