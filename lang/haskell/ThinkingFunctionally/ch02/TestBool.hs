to :: Bool -> Bool
to b = not (to b)

-- data Bool = True | False | undefined
-- 所有的类型定义，都会包含undefined
-- 为了支持lazy evaluation
