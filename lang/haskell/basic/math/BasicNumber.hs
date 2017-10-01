module BasicNumber where

factorial 0 = 1
factorial n = n * factorial (n - 1)

factorial1 n = foldl1 (*) [1..n]

permuatation n x = foldl1 (*) [(x + 1)..n]

combination n x = (permuatation n x) / (factorial (n - x))
