--------------------------------------------------------------------------------
-- Applicative typeclass
-- It lies in the Control.APplicative module
-- 
-- (<$>) :: (Functor f) => (a -> b) -> f a -> f b
-- f <$> x = fmap f x
-- 
-- class (Functor f) => Applicative f where
--         pure :: a -> f a
--         (<*>) :: f (a -> b) -> f a -> f b
-- 
-- instance Applicative [] where
--         pure x = [x]
--         fs <*> xs = [f x | f <- fs, x <- xs] 
-- ref: http://learnyouahaskell.com/functors-applicative-functors-and-monoids
--------------------------------------------------------------------------------

x = filter (>50) $ (*) <$> [2,5,10] <*> [8,10,11]
main = print x
