type Birds = Int
type Pole = (Birds, Birds)

check :: Pole -> Maybe Pole
check (left, right)
        | abs (left - right) < 4 = Just (left, right)
        | otherwise = Nothing

landLeft :: Birds -> Pole -> Maybe Pole
landLeft n (left, right) = check (left + n, right)

landRight :: Birds -> Pole -> Maybe Pole
landRight n (left, right) = check (left, right + n)

banana _ = Nothing

-- Maybe is a Monad, which handle failure
ret0 = return (0, 0) >>= landLeft 1 >>= landRight 1 >>= landLeft 2
ret1 = return (0, 0) >>= landLeft 5 >>= landRight 1 >>= landLeft (-2)
ret2 = return (0, 0) >>= landLeft 1 >>= banana >>= landLeft 2
{-
    (>>) :: (Monad m) => m a -> m b -> m b
    m >> n = m >>= \_ -> n
-}
-- use >>, we can directly make failure
ret3 = return (0, 0) >>= landLeft 1 >> Nothing >>= landLeft 2

main = do print ret0
          print ret1
          print ret2
          print ret3
