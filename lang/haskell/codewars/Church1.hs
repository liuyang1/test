module Haskell.Codewars.Church1 where
-- import Haskell.Codewars.Church.Preloaded -- Remove before submit

import Data.Function (on)
(.:) = ( . ) . ( . )

type Lambda a = (a -> a)
type Cnum a = Lambda a -> Lambda a

zero :: Cnum a
zero f = id
one :: Cnum a
one f = f

churchSucc :: Cnum a -> Cnum a
churchSucc c h = c h . h

two = churchSucc one
thr = churchSucc two

toChurch 0 = zero
toChurch n = churchSucc (toChurch (n - 1))

inc x = x + 1

toInt c = (c inc) 0

churchAdd :: Cnum a -> Cnum a -> Cnum a
churchAdd c1 c2 h = c1 h . c2 h

churchMul :: Cnum a -> Cnum a -> Cnum a
churchMul c1 c2 h = c1 (c2 h)

--Extra credit: Why is the type signature different?
churchPow :: Cnum a -> (Cnum a -> Cnum a) -> Cnum a
churchPow cb ce = ce cb

wrap cf = toInt .: (cf `on` toChurch)
add = wrap churchAdd
mul = wrap churchMul
pow1 x y = toInt (churchPow (toChurch x) (toChurch y))
-- pow2 = wrap churchPow
-- This limit churchPow's two input argument have same type
-- However, they are not.

main = do
    print $ toInt (churchAdd two thr)
    print $ toInt (churchMul two thr)
    print $ toInt (churchPow two thr)
    print $ add 2 3
    print $ mul 4 5
    print $ pow1 4 5
