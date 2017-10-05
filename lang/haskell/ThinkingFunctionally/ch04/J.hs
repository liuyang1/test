import Data.List (sort)

fork :: (a -> b, a -> c) -> a -> (b, c)
fork (f, g) x = (f x, g x)

a0 f n = map f . take n
a1 f n = take n . map f
-- 一致

b0 f = map f . reverse
b1 f = reverse . map f
-- 一致

c0 f = map f . sort
c1 f = sort . map f
-- 不一致

d0 f p = map f . filter p
d1 f p = map fst . filter snd . map (fork (f, p))
-- 一致

e0 p g = filter (p . g)
-- e1 p g = map (invertg) . filter p . map g
-- 一致

f0 = reverse . concat
f1 = concat . reverse . map reverse
xs = [[1, 2], [3, 4]]
test = f0 xs == f1 xs
-- left = [4, 3, 2, 1]
-- right = concat . reverse $ [[2, 1], [4, 3]]
--       = concat . [[4, 3], [2, 1]]
--       = [4, 3, 2, 1]
-- 一致

h0 p = filter p . concat
h1 p = concat . map (filter p)
-- 一致
