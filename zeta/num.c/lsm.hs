import Data.List (genericLength)
mean xs = sum xs / genericLength xs


lsm xs ys = ((ym - k * xm), k)
    where xm = mean xs
          ym = mean ys
          x_ = [x - xm | x <- xs]
          y_ = [y - ym | y <- ys]
          xy = sum $ zipWith (*) x_ y_
          xx = sum $ zipWith (*) x_ x_
          k = xy / xx

h xs ys = lsm (drop 3 (take nn xs))
              (drop 3 (take nn ys))
    where nn = min (length xs) (length ys)

ns = [500, 700, 1000, 1400, 2000, 2800, 4000, 5600, 8000, 11200, 16000, 22400, 32000]
-- simple
x0 = [4.92, 9.16, 20.53, 37.90, 83.86, 152.56, 331.10]
-- mul
x1 = [2.75, 5.32, 11.08, 21.68, 44.31, 87.54, 177.85,
     349.21, 718.83]
-- kara
x2 = [1.14, 2.12, 3.56, 6.42, 10.84, 19.46, 32.71,
     58.56, 99.03, 173.10, 294.04, 531.78]
-- cheat
x3 = [0.03, 0.05, 0.06, 0.08, 0.13, 0.24, 0.42, 0.74, 1.46,
     2.96, 6.17, 13.35, 29.54]
-- toom3
x4 = [1.63, 2.43, 4.78, 6.17, 12.65, 23.79, 31.09, 52.18, 113.94,
     148.31, 257.66]

mlog = map log

ns_ = mlog ns

-- 2.02, 2.01, 1.61
--ks = map (\i -> snd $ lsm (drop 0 ns_) (drop 0 $ (mlog i))) [x0, x1, x2, x3, x4]
ks = map (\i -> snd $ h ns_ (mlog i)) [x0, x1, x2, x3, x4]
