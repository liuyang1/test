import Data.List

move = do
        (x, y) <- [(1, 2)]
        sign0 <- [1, -1]
        sign1 <- [1, -1]
        (a, b) <- [(sign0 * x, sign1 * y)]
        rt <- [(a, b), (b, a)]
        return rt

type KnightPos = (Int, Int)

guard True = return ()
guard False = []

moveKnight :: KnightPos -> [KnightPos]
moveKnight (c,r) = do
    (x, y) <- move
    (c',r') <- [(c + x, r - y)]
    guard (c' `elem` [1..8] && r' `elem` [1..8])
    return (c',r')

moveKnight3 start = nub $ return start >>= moveKnight >>= moveKnight >>= moveKnight

reach pos lst = pos `elem` lst
main = do print $ moveKnight (1, 2)
          print $ reach (7, 7) $ moveKnight3 (1, 2)
          print $ reach (6, 6) $ moveKnight3 (1, 2)
