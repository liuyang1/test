module Evolution where
{-# LANGUAGE MultiParamTypeClasses #-}
{-# LANGUAGE FunctionalDependencies #-}
{-# LANGUAGE RankNTypes #-}
-- Freshman Haskell programmer
-- >>> naive recursion function
fac0 n = if n == 0
            then 1
            else n * fac0 (n-1)

-- Sophomore Haskell programmer, at MIT
-- (studied Scheme as a freshman)
-- >>> lambda & bracket () in lisp style in MIT
fac1 = (\(n) ->
        (if ((==) n 0)
            then 1
            else ((*) n (fac1 ((-) n 1)))))

-- Junior Haskell programmer
-- (beginning Peano player)
-- >>> Pattern matching style
-- >>> How to dervied from N to N + 1
-- >>> WARNING: this pattern is unsupported now
-- fac2  0    =  1
-- fac2 (n+1) = (n+1) * fac2 n

-- Another junior Haskell programmer
-- (read that n+k patterns are “a disgusting part of Haskell” [1]
-- and joined the “Ban n+k patterns”-movement [2])
-- >>> Don't use `N+K` pattern in Haskell
fac3 0 = 1
fac3 n = n * fac3 (n-1)

-- Senior Haskell programmer
-- (voted for   Nixon   Buchanan   Bush — “leans right”)
-- >>> learn fold style
fac4 n = foldr (*) 1 [1..n]

-- Another senior Haskell programmer
-- (voted for   McGovern   Biafra   Nader — “leans left”)
-- >>> fold style, and distinguish foldl and foldr
fac5 n = foldl (*) 1 [1..n]

-- Yet another senior Haskell programmer
-- (leaned so far right he came back left again!)
-- using foldr to simulate foldl
-- >>> complex understand on fold function
-- >>> you can consider G as a continution
fac6 n = foldr (\x g n -> g (x*n)) id [1..n] 1

-- Memoizing Haskell programmer
-- (takes Ginkgo Biloba daily)
-- >>> calculate factorials one by one
facs = scanl (*) 1 [1..]
-- >>> directly get result from infinite list facs
-- >>> it have memorized result in facs
fac7 n = facs !! n

-- Pointless (ahem) “Points-free” Haskell programmer
-- (studied at Oxford)
-- >>> pointless style, use partial function composition to avoid argument
-- >>> fac8 n = foldr (*) 1 $ enumFromTo 1 n = (foldr (*) 1 . enumFromTo 1) n
fac8 = foldr (*) 1 . enumFromTo 1

-- Iterative Haskell programmer
-- (former Pascal programmer)
-- >>> create a FOR loop with
-- >>>      - init status,
-- >>>      - next iterate function,
-- >>>      - done stop condtion
-- >>>      - result: probe result from status
fac9 n = result (for init next done)
        where init = (0,1)
              next   (i,m) = (i+1, m * (i+1))
              done   (i,_) = i==n
              result (_,m) = m

for i n d = until d n i

-- Iterative one-liner Haskell programmer
-- (former APL and C programmer)
-- >>> This one quitly like Pascal style, but directly with function composition
fac10 n = snd (until ((>n) . fst) (\(i,m) -> (i+1, i*m)) (1,1))

-- Accumulating Haskell programmer
-- (building up to a quick climax)
-- >>> add accumlator status as helper function's argument
facAcc a 0 = a
facAcc a n = facAcc (n*a) (n-1)

fac11 = facAcc 1

-- Continuation-passing Haskell programmer
-- (raised RABBITS in early years, then moved to New Jersey)
-- >>> k is continutions
-- >>> assusme pass argument = 3
-- >>> KK = id . (3 *) . (2 *) (1 *) 1
-- >>>    = 6
facCps k 0 = k 1
facCps k n = facCps (k . (n *)) (n-1)

fac12 = facCps id

-- Boy Scout Haskell programmer
-- (likes tying knots; always “reverent,” he
-- belongs to the Church of the Least Fixed-Point [8])
-- >>> Y combinator
-- >>> y :: (t -> t) -> t
y g = g (y g)
-- >>> write a self-recursion function without function's NAME
-- >>> fac13 3 = (y ()) 3
--             = (\..) (y f) 3
--             = (n * f (n - 1))
--             = 3 * ((y f) 2)
--             = 3 * (f (y f) 2)
--             = 3 * 2 * ((y f) 1)
--             = ...
--             = 6
-- f :: Int -> Int
-- g :: (Int -> Int) -> Int -> Int
-- g = \f n ->if (n==0) then 1 else n * f (n-1)
fac13 = y (\f n -> if (n==0) then 1 else n * f (n-1))



-- Combinatory Haskell programmer
-- (eschews variables, if not obfuscation;
-- all this currying’s just a phase, though it seldom hinders)
s f g x = f x (g x)

k x y   = x

b f g x = f (g x)

c f g x = f x g

-- duplicate definition
-- y f     = f (y f)

-- split to perquisite, f, gfunction
cond p f g x = if p x then f x else g x

fac14  = y (b (cond (0 ==) (k 1)) (b (s (*)) (c b pred)))

-- List-encoding Haskell programmer
-- (prefers to count in unary)
arb = ()    -- "undefined" is also a good RHS, as is "arb" :)

-- >>> handling number based on list
-- >>> enc -> map value to list
-- >>> prod -> multiplcate in list
-- >>> prj -> project list back to value
listenc n = replicate n arb
listprj f = length . f . listenc

listprod xs ys = [ i (x,y) | x<-xs, y<-ys ]
                 where i _ = arb

facl []         = listenc  1
facl n@(_:pred) = listprod n (facl pred)

fac15 = listprj facl

-- Interpretive Haskell programmer
-- (never “met a language” he didn't like)
-- a dynamically-typed term language
-- >>> This part implement a dymamic-typed language

data Term = Occ Var
          | Use Prim
          | Lit Integer
          | App Term Term
          | Abs Var  Term
          | Rec Var  Term

type Var  = String
type Prim = String


-- a domain of values, including functions

data Value = Num  Integer
           | Bool Bool
           | Fun (Value -> Value)

instance Show Value where
  show (Num  n) = show n
  show (Bool b) = show b
  show (Fun  _) = ""

prjFun (Fun f) = f
prjFun  _      = error "bad function value"

prjNum (Num n) = n
prjNum  _      = error "bad numeric value"

prjBool (Bool b) = b
prjBool  _       = error "bad boolean value"

-- >>> map a ordinary function f to INJ Domain's Func
binOp inj f = Fun (\i -> (Fun (\j -> inj (f (prjNum i) (prjNum j)))))


-- environments mapping variables to values
-- >>> environment model
type Env = [(Var, Value)]

getval x env =  case lookup x env of
                  Just v  -> v
                  Nothing -> error ("no value for " ++ x)


-- an environment-based evaluation function

eval env (Occ x) = getval x env
eval env (Use c) = getval c prims
eval env (Lit k) = Num k
eval env (App m n) = prjFun (eval env m) (eval env n)
eval env (Abs x m) = Fun  (\v -> eval ((x,v) : env) m)
eval env (Rec x m) = f where f = eval ((x,f) : env) m


-- a (fixed) "environment" of language primitives

times = binOp Num  (*)
minus = binOp Num  (-)
equal = binOp Bool (==)
cond16  = Fun (\b -> Fun (\x -> Fun (\y -> if (prjBool b) then x else y)))

prims = [ ("*", times), ("-", minus), ("==", equal), ("if", cond16) ]


-- a term representing factorial and a "wrapper" for evaluation

facTerm = Rec "f" (Abs "n"
              (App (App (App (Use "if")
                   (App (App (Use "==") (Occ "n")) (Lit 0))) (Lit 1))
                   (App (App (Use "*")  (Occ "n"))
                        (App (Occ "f")
                             (App (App (Use "-") (Occ "n")) (Lit 1))))))

fac16 n = prjNum (eval [] (App facTerm (Lit n)))

-- Static Haskell programmer
-- (he does it with class, he’s got that fundep Jones!
-- After Thomas Hallgren’s “Fun with Functional Dependencies” [7])
-- static Peano constructors and numerals

{- >>> I cannot make GHC accept this solution
data Zero
data Succ n

type One   = Succ Zero
type Two   = Succ One
type Three = Succ Two
type Four  = Succ Three


-- >>> It calculate based on type, but not value
-- dynamic representatives for static Peanos

zero  = undefined :: Zero
one   = undefined :: One
two   = undefined :: Two
three = undefined :: Three
four  = undefined :: Four


-- addition, a la Prolog

class Add1 a b c | a b -> c where
  add :: a -> b -> c

instance              Add1  Zero    b  b
instance Add1 a b c => Add1 (Succ a) b (Succ c)


-- multiplication, a la Prolog

class Mul a b c | a b -> c where
  mul :: a -> b -> c

instance                           Mul  Zero    b Zero
instance (Mul a b c, Add1 b c d) => Mul (Succ a) b d


-- factorial, a la Prolog

class Fac a b | a -> b where
  fac :: a -> b

instance                                Fac  Zero    One
instance (Fac n k, Mul (Succ n) k m) => Fac (Succ n) m

-- try, for "instance" (sorry):
--
--     :t fac four
-}

-- Beginning graduate Haskell programmer
-- (graduate education tends to liberate one from petty concerns
-- about, e.g., the efficiency of hardware-based integers)
-- the natural numbers, a la Peano

data Nat = Zero | Succ Nat
    -- deriving (Show)

-- iteration and some applications

iter z s  Zero    = z
iter z s (Succ n) = s (iter z s n)

plus n = iter n     Succ
mult n = iter Zero (plus n)


-- primitive recursion

primrec z s  Zero    = z
primrec z s (Succ n) = s n (primrec z s n)


-- two versions of factorial

fac18  = snd . iter (one, one) (\(a,b) -> (Succ a, mult a b))
fac18' = primrec one (mult . Succ)

int = iter 0 (1+)

-- instance Show Nat where
--   show = show . int

(zero : one : two : three : four : five : _) = iterate Succ Zero
-- for convenience and testing (try e.g. "fac five")


-- Origamist Haskell programmer
-- (always starts out with the “basic Bird fold”)
-- (curried, list) fold and an application

fold c n []     = n
fold c n (x:xs) = c x (fold c n xs)

prod = fold (*) 1


-- (curried, boolean-based, list) unfold and an application

unfold p f g x =
  if p x
     then []
     else f x : unfold p f g (g x)

-- pred :: Enum a => a -> a
-- substract 1
downfrom = unfold (==0) id pred


-- hylomorphisms, as-is or "unfolded" (ouch! sorry ...)

refold  c n p f g   = fold c n . unfold p f g

refold' c n p f g x =
  if p x
     then n
     else c (f x) (refold' c n p f g (g x))


-- several versions of factorial, all (extensionally) equivalent

fac19   = prod . downfrom
fac19'  = refold  (*) 1 (==0) id pred
fac19'' = refold' (*) 1 (==0) id pred

-- Cartesianally-inclined Haskell programmer
-- (prefers Greek food, avoids the spicy Indian stuff;
-- inspired by Lex Augusteijn’s “Sorting Morphisms” [3])
-- (product-based, list) catamorphisms and an application

-- >>> fold with ()
cata :: (i, (a, i) -> i) -> [a] -> i
cata (n,c) []     = n
cata (n,c) (x:xs) = c (x, cata (n,c) xs)

mult1 = uncurry (*)
prod1 = cata (1, mult1)


-- (co-product-based, list) anamorphisms and an application

ana f = either (const []) (cons . pair (id, ana f)) . f

cons = uncurry (:)

downfrom1 = ana uncount

uncount 0 = Left  ()
uncount n = Right (n, n-1)


-- two variations on list hylomorphisms

hylo  f  g    = cata g . ana f

-- hylo' f (n,c) = either (const n) (c . pair (id, hylo' f (c,n))) . f

pair :: (a->b, c->d) -> (a, c) -> (b, d)
pair (f,g) (x,y) = (f x, g y)


-- several versions of factorial, all (extensionally) equivalent

fac20   = prod1 . downfrom1
fac20'  = hylo  uncount (1, mult1)
-- fac20'' = hylo' uncount (1, mult1)

-- Ph.D. Haskell programmer
-- (ate so many bananas that his eyes bugged out, now he needs new lenses!)
-- explicit type recursion based on functors
{-

newtype Mu f = Mu (f (Mu f))  deriving Show

inn     x  = Mu x
out (Mu x) = x


-- cata- and ana-morphisms, now for *arbitrary* (regular) base functors

cata1 phi = phi . fmap (cata1 phi) . out
ana1  psi = inn  . fmap (ana1  psi) . psi


-- base functor and data type for natural numbers,
-- using a curried elimination operator

data N b = Zero1 | Succ1 b  deriving Show

instance Functor N where
  fmap f = nelim Zero1 (Succ1 . f)

nelim z s  Zero1    = z
nelim z s (Succ1 n) = s n

type Nat1 = Mu N


-- conversion to internal numbers, conveniences and applications

int1 = cata1 (nelim 0 (1+))

instance Show Nat1 where
  show = show . int1

zero1 = inn   Zero1
suck = inn . Succ1       -- pardon my "French" (Prelude conflict)

plus2 n = cata1 (nelim n     suck   )
mult2 n = cata1 (nelim zero (plus2 n))


-- base functor and data type for lists

data L a b = Nil | Cons a b  deriving Show

instance Functor (L a) where
  fmap f = lelim Nil (\a b -> Cons a (f b))

lelim n c  Nil       = n
lelim n c (Cons a b) = c a b

type List a = Mu (L a)


-- conversion to internal lists, conveniences and applications

list = cata1 (lelim [] (:))

instance Show a => Show (List a) where
  show = show . list

prod2 = cata1 (lelim (suck zero1) mult2)

upto = ana1 (nelim Nil (diag (Cons . suck)) . out)

diag f x = f x x

fac21 = prod2 . upto
-}
{-
-- Post-doc Haskell programmer
-- (from Uustalu, Vene and Pardo’s “Recursion Schemes from Comonads” [4])
-- explicit type recursion with functors and catamorphisms

newtype Mu1 f = In (f (Mu1 f))

unIn (In x) = x

cata2 phi = phi . fmap (cata2 phi) . unIn


-- base functor and data type for natural numbers,
-- using locally-defined "eliminators"

data N1 c = Z | S c

instance Functor N1 where
  fmap g  Z    = Z
  fmap g (S x) = S (g x)

type Nat1 = Mu1 N1

zero2   = In  Z
suck2 n = In (S n)

add m = cata2 phi where
  phi  Z    = m
  phi (S f) = suck2 f

mult3 m = cata2 phi where
  phi  Z    = zero2
  phi (S f) = add m f


-- explicit products and their functorial action

data Prod e c = Pair c e

outl (Pair x y) = x
outr (Pair x y) = y

fork f g x = Pair (f x) (g x)

instance Functor (Prod e) where
  fmap g = fork (g . outl) outr


-- comonads, the categorical "opposite" of monads

class Functor n => Comonad n where
  extr :: n a -> a
  dupl :: n a -> n (n a)

instance Comonad (Prod e) where
  extr = outl
  dupl = fork id outr


-- generalized catamorphisms, zygomorphisms and paramorphisms

gcata :: (Functor f, Comonad n) =>
           (forall a. f (n a) -> n (f a))
             -> (f (n c) -> c) -> Mu1 f -> c

gcata dist phi = extr . cata (fmap phi . dist . fmap dupl)

zygo chi = gcata (fork (fmap outl) (chi . fmap outr))

para :: Functor f => (f (Prod (Mu1 f) c) -> c) -> Mu1 f -> c
para = zygo In


-- factorial, the *hard* way!

fac22 = para phi where
  phi  Z             = suck2 zero2
  phi (S (Pair f n)) = mult3 f (suck2 n)


-- for convenience and testing

int2 = cata phi where
  phi  Z    = 0
  phi (S f) = 1 + f

instance Show (Mu1 N1) where
  show = show . int2
-}

-- Tenured professor
-- (teaching Haskell to freshmen)
fac23 n = product [1..n]
