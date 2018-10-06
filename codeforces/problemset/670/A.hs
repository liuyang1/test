module Main where

off n = (2 * week + max 0 (r - 5), 2 * week + min 2 r)
    where (week, r) = n `divMod` 7

main = do
    l <- getLine
    let n = read l::Integer
        r = off n
    putStrLn $ unwords [show $ fst r, show $ snd r]
