nb_year' p0 pre aug thr c = if p0 >= thr then c else nb_year' (p0 * (1 + pre) + aug) pre aug thr (c + 1)

nbYear :: Int -> Double -> Int -> Int -> Int
nbYear p0 pre aug thr = nb_year' p0 (pre/100) aug thr 0
