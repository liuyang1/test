{-# LANGUAGE RankNTypes #-}
-- first line, defined for GHC compiler.
module Haskell.Codewars.Church where
import Prelude hiding (succ, pred, fst, snd)
--------------------------------------------------------------------------------
-- Church Number
-- define number with function.
-- ref:https://karczmarczuk.users.greyc.fr/Essays/church.html

newtype Number = Nr (forall a. (a -> a) -> a -> a)

-- zero= Nr (\ s z -> z)
-- one = succ zero
-- one = Nr (\ s z -> s (zero s z))
--     = Nr (\ s z -> s z)
-- two = Nr (\ s z -> s (one s z))
--     = Nr (\ s z -> s (s z))
-- nnn = Nr (\ s z -> s (s .. (s z)))
--                    +--------+ n
-- Number n : apply function S on Z N th times
zero :: Number
zero = Nr (\ _ z -> z)

succ :: Number -> Number
-- add one more time S function apply
succ (Nr a) = Nr (\ s z -> s (a s z))

suc (_, next) = (next, succ next)
pred :: Number -> Number
pred (Nr a) = x
  where (x, _) = a suc (zero, zero)
-- apply function SUC on (zero, zero) N times, then get first slice, which
-- store last value, which is N - 1

add :: Number -> Number -> Number
-- apply function SUCC on B A times, then get A + B
add (Nr a) = a succ

subt :: Number -> Number -> Number
-- apply function PRED on A B times, then get A - B
subt a (Nr b) = b pred a

newtype Pair a b = Pr (forall c . (a -> b -> c) -> c)

instance (Show a, Show b) => Show (Pair a b) where
  show (Pr p) = p (\ a b -> "(" ++ show a ++ "," ++ show b ++ ")")

pair :: a -> b -> Pair a b
pair f s = Pr (\ b -> b f s)

fst :: Pair a b -> a
fst (Pr p) = p (\ a _ -> a)

snd :: Pair a b -> b
snd (Pr p) = p (\ _ b -> b)

instance Show Number where
  show (Nr a) = a ("1+" ++) "0"

instance Eq Number where
  a == b = eval a == eval b

fold :: Number -> (a -> a) -> a -> a
fold (Nr n) s z = n s z

eval :: Number -> Integer
eval (Nr a) = a (+1) 0

mult :: Number -> Number -> Number
-- apply ADD B on ZERO B times, then get A * B
mult (Nr a) b =  a (add b) zero
