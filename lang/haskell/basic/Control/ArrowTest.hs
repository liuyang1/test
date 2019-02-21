import           Control.Arrow
import           Data.List     (group, sort)

-- arr :: (b -> c) -> a b c
-- lift a function to an arrow
-- 我们可以认为这个函数实际上没有发生任何变化,只是认为的Arrow范畴里面而已
t1 = arr (+ 2) $ 1
-- 3

-- first :: a b c -> a (b, d) (c, d)
-- first :: (b -> c) -> (b, d) -> (c, d)
-- apply function on first part, and keep reset unchanged
t2 = first (+ 2) (1, 3)
-- (3, 3)

-- ((&&&)) :: Arrow a => a b c -> a b c' -> a b (c, c')
-- ((&&&)) :: (b -> c) -> (b -> c') -> b -> (c, c')
a = map (head &&& length) . group . sort $ "hello world!"

-- ((***)) :: Arrow a => a b c -> a b' c' -> a (b, b') (c, c')
-- ((***)) :: (b -> c) -> (b' -> c') -> (b, b') -> (c, c')
-- 笛卡尔积上类型的分别运算
move (x, y) = ((+ x) *** (+ y))

b = (1, 1) `move` (3, 4)

-- ((+++)) :: a b c -> a b' c' -> a (Either b b') (Either c c')
-- apply two function on Either value, get one Either new value
f = (+ 1) +++ (`div` 2)
t3 = (f (Left 4), f (Right 4))
-- (Left 5, Right 2)

-- ((<+>))
