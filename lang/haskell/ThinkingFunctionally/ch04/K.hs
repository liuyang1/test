fork :: (a -> b, a -> c) -> a -> (b, c)
fork (f, g) x = (f x, g x)

myunzip :: [(a, b)] -> ([a], [b])
myunzip = fork (map fst, map snd)

cross :: (a -> c, b -> d) -> (a, b) -> (c, d)
cross (f, g) = fork (f . fst, g . snd)

--------------------------------------------------------------------------------
-- 证明
-- LEFT = cross (map f, map g) . unzip $ [(A, B)]
--      = cross (map f, map g) ([A], [B])
--      = fork (map f . fst, map g . snd) ([A], [B])
--      = ((map f . fst $ ([A], [B])), (map g . fst $ ([A], [B])))
--      = (map f [A], map g [B])
--      = ([f A], [g B])
-- RIGHT = unzip . (map (cross (f, g))) $ [(A, B)]
--       = unzip $ (map (cross (f, g)) [(A, B)])
--       = unzip $ ([fork ((f . fst), (g . snd)) (A, B)])
--       = unzip $ ([(f A, g B)])
--       = unzip [(f A, g B)]
--       = fork (map fst, map snd) [(f A, g B]
--       = ([f A], [g B])


--------------------------------------------------------------------------------
-- Law
-- cross (f, g) . fork (h, k) = fork (f . h, g . k)
-- fork (f, g) . h = fork (f . h, g . h)
-- fst . cross (f, g) = f . fst
-- snd . cross (f, g) = g . snd

--------------------------------------------------------------------------------
-- 证明
-- LEFT = cross (map f, map g) . unzip
--      = cross (map f, map g) . fork (map fst, map snd)
--      = fork (map f . map fst, map g . map snd) -- cross, fork 定律
--      = fork (map (f . fst), map (g . snd)) -- map law
-- RIGHT = fork (map fst, map snd) . (map (cross (f, g)))
--       = fork (map fst . map (cross (f, g), map snd . map (cross (f, g)))) -- fork law
--       = fork (map (fst . cross (f, g)), map snd . cross (f, g))
--       = fork (map (f . fst), map (g . snd)) -- map law
-- so LEFT == RIGHT
