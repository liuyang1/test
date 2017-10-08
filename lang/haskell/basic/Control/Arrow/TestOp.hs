import Data.List (sort, group)
import Control.Arrow

-- ((&&&)) :: Arrow a => a b c -> a b c' -> a b (c, c')
-- ((&&&)) :: (b -> c) -> (b -> c') -> b -> (c, c')
a = map (head &&& length) . group . sort $ "hello world!"

-- ((***)) :: Arrow a => a b c -> a b' c' -> a (b, b') (c, c')
-- ((***)) :: (b -> c) -> (b' -> c') -> (b, b') -> (c, c')
-- 笛卡尔积上类型的分别运算
move (x, y) = ((+ x) *** (+ y))

b = (1, 1) `move` (3, 4)

-- ((+++))
-- ((<+>))
