-- Variation zero: Standard interpreter
type I a = a
unitI a = a
a `bindI` k = k a
showI a = showval a

-- Variation one: Error message
data E a = Success a | Error String

unitE a = Success a
errorE s = Error s

(Success a) `bindE` k = k a
(Error s) `bindE` k = Error s

showE (Success a) = "Success: " ++ showval a
showE (Error s) = "Error: " ++ s


M = I
unitM = unitI
bindM = bindI
showM = showI

-- implement interpreter
type Name = String
data Term = Var Name
          | Con Int
          | Add Term Term
          | Lam Name Term
          | App Term Term

data Value = Wrong
           | Num Int
           | Fun (Value -> M Value)

type Environment = [(Name, Value)]

showval :: Value -> String
showval Wrong = "<wrong>"
showval (Num i) = showint i
showval (Fun f) = "<function>"

interp :: Term -> Environment -> M Value
interp (Var x) e = lookupenv x e
interp (Con i) e = unitM (Num i)
interp (Add u v) e = interp u e `bindM` (\a -> interp v e `bindM` (apply f))
interp (Lam x v) e = unitM (Fun (\a -> interp v ((x, a):e)))
interp (App t u) e = interp t e `bindM` (\f -> interp u e `bindM` (apply f))

lookupenv :: Name -> Environment -> M Value
lookupenv x [] = unitM Wrong
lookupenv x ((y, b): e) = if x == y then unitM b else lookupenv x e

add :: Value -> Value -> M Value
add (Num i) (Num j) = unitM (Num (i + j))
add a b = unitM Wrong

apply :: Value -> Value -> M Value
apply (Fun k) a = k a
apply f a = unitM Wrong

test :: Term -> String
test t = showM (interp t [])

term0 = (App (Lam "x" (Add (Var "x") (Var "x")))
             (Add (Con 10) (Con 11)))

main = test term0
