import Control.Monad.Instances

addStuff :: Int -> Int
addStuff = do
    a <- (* 2)
    b <- (+ 10)
    return (a+b)

addStuff2 :: Int -> Int
addStuff2 x = let
    a = (*2) x
    b = (+10) x
 in (a+b)
