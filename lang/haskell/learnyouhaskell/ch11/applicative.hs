{-
    class (Functor f) => Applicative f where
        pure :: a -> f a
        (<*>) :: f (a -> b) -> f a -> f b
    Applicative can over Maybe, IO, List, ((->) r)
-}
import Control.Applicative

ret0 = fmap (\x-> x 9) $ fmap (*) [1..4]

-- Applicative over []
ret1 = (*) <$> [1, 2, 3] <*> [4, 5, 6]
ret2 = [(*), (+)] <*> [1, 2, 3] <*> [4, 5, 6]

-- Applicative over ((->) r)
ret3 = (+) <$> (+3) <*> (*100) $ 5
-- (\x, y -> x + y) <$> (\x -> x + 3) <*> (\x -> x * 100)
-- (\x -> (x + 3) + (x * 100))
ret4 = (\x y z -> [x,y,z]) <$> (+3) <*> (*2) <*> (/2) $ 5

main = do print ret0
          print ret1
          print ret2
          print ret3
          print ret4
