module Checkerboard where

fn x = "[" ++ (if even x then "r" else "b") ++ "]"
checkerboard :: Int -> String
checkerboard n = let xs = [0..(n - 1)]
                     evenr = xs
                     oddr = map (+ 1) xs
                  in concatMap ((++ "\n") . concatMap fn .  (\x -> if even x then evenr else oddr)) xs
