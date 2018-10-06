module Main where

dist (x0, y0) (x1, y1) = sqrt((x1 - x0) ^ 2 + (y1 - y0) ^ 2)

edge (ap, bp, cp) = (dist bp cp, dist cp ap, dist bp ap)
angle (a, b, c) = (deAngle a b c, deAngle b c a, deAngle c a b)

helen (a, b, c) = sqrt(s * (s - a) * (s - b) * (s - c))
    where s = (a + b + c) / 2

raidus (a, b, c) = a * b * c / 4 / helen (a, b, c)

deAngle a b c = acos $ (b ^ 2 + c ^ 2 - a ^ 2) / (2 * b * c)

eps :: Double
eps = 0.01
{- KEY of this issue
-}
fgcd :: Double -> Double -> Double
fgcd a b
  | abs a <= eps || abs b <= eps = a + b
  | a >= b = fgcd (a - b * fromInteger (floor (a / b))) b
  | otherwise = fgcd b a

fgcd3 (x, y, z) = fgcd (fgcd x y) z

ployN edges = pi / fgcd3 (angle edges)

areaN r n = n / 2 * r ^ 2 * sin(2 * pi / n)

sln pt = areaN r n
    where e = edge pt
          n = ployN e
          r = raidus e

getPoint = do
    l <- getLine
    let xs = words l
        ys = map (\w->read w::Double) xs
    return (ys!!0, ys!! 1)
main = do
    ap <- getPoint
    bp <- getPoint
    cp <- getPoint
    print $ sln (ap, bp, cp)
