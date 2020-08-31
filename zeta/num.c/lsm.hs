import Data.List (genericLength)
mean xs = sum xs / genericLength xs

ratio xs ys = xy / xx
    where xm = mean xs
          ym = mean ys
          x_ = [x - xm | x <- xs]
          y_ = [y - ym | y <- ys]
          xy = sum $ zipWith (*) x_ y_
          xx = sum $ zipWith (*) x_ x_

ns = [500, 700, 1000, 1400, 2000, 2800, 4000]
x0 = [4.92, 9.16, 20.53, 37.90, 83.86, 152.56, 331.10]
x1 = [2.75, 5.32, 11.08, 21.68, 44.31, 87.54, 177.85]
x2 = [1.14, 2.12, 3.56, 6.42, 10.84, 19.46, 32.71]

mlog = map log

ns_ = mlog ns

-- 2.02, 2.01, 1.61
k0 = ratio ns_ (mlog x0)
k1 = ratio ns_ (mlog x1)
k2 = ratio ns_ (mlog x2)
