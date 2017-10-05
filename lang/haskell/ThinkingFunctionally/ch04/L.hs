-- cross (f, g) . cross (h, k) = cross (f . h, g . k)
-- LEFT = cross (f, g) . fork (h . fst, k . snd)
--      = fork (f . h . fst, g . k . snd)
-- RIGHT = fork (f . h . fst, g . k . snd)

-- cross (id, id) = id
-- cross bifunctor 双函子

class MyBifunctor p where
    bimap :: (a -> b) -> (c -> d) -> p a c -> p b d

type Pair a b = (a, b)

-- TODO: 不知道该如何定义
-- instance MyBifunctor Pair where
--     bimap f g (a, b) = (f a, g b)

instance MyBifunctor Either where
    bimap f g (Left x) = Left (f x)
    bimap f g (Right y) = Right (g y)
