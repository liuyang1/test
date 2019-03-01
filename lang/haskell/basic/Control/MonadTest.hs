import Control.Monad

-- >=> :: Monad m => (a -> m b) -> (b -> m c) -> (a -> m c)
-- Left-to-right composition of Kleisli arrows
-- 'fish'
mod2 x = if x `mod` 2 == 0 then Just x else Nothing
mod3 x = if x `mod` 3 == 0 then Just x else Nothing
mod6 = mod2 >=> mod3

t0 = do
    print $ mod6 3
    print $ mod6 2
    print $ mod6 6

bin2 x = [x/2, 3*x+1]
binB = bin2 >=> bin2
t1 = print $ binB (4::Rational)
    -- 4 -> [2,13] -> [1,7,13/2,40]
