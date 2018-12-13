{- 

HASKELL BASICS - CSE 341

Instead of the usual Powerpoint slides or web notes, for Haskell, 
the lecture notes can be run!

This stuff is a comment.  (You probably figured that out already.)
Big comments can be delimited by {-   ... stuff ... -}

{- These symbols nest, so this is a comment within a comment ... -}

You can also put a comment on one line, or part of a line: everything on
that line after the -- is a comment.

This file is linked from the 341 website.  To load it into Haskell,
save it under the name HaskellBasics.hs (make sure it doesn't have a .txt
extension).  Then start Haskell from the command line and load the file:
   ghci HaskellBasics.hs

Or on Windows you can double click on the file icon.


Once in Haskell, just type an expression to evaluate it.  To see the type
of an expression without evaluating it, use this:
   :type expr
or just
   :t expr

You can also load a file from within Haskell using
   :load MyFile
or just
   :l MyFile

If you've subsequently edited the file with an external editor, use
   :reload 
or
   :r
to reload the last loaded file.

Another useful Haskell command is
   :set +t
which causes Haskell to print type information after evaluation.  See the
"Running Haskell" web page or the online manuals for more info.

   :quit or :q exits Haskell 

*********************************

Haskell is an example of a pure functional programming language -- there
are no side effects.

Imperative languages have variables and assignment; they are in effect
abstractions of the von Neumann machine. There is no referential
transparency.

In mathematics, we can substitute equals for equals:
if x=y+z then you can substitute y+z for x 

Informally, in a referentially transparent language you can substitute
equals for equals, just as in mathematics.

Interesting features of Haskell:

    truly functional 
    lazy evaluation -- can deal with infinite structures 
    type system -- statically typed, no type declarations needed; polymorphic 


Future of functional languages

  Barriers to adoption: 
    efficiency (a declining problem; also functional languages good
              candidates for parallel execution) 
    programmer acceptance (although Haskell is gaining ground)
    arguably unnatural for some things, such as interactive I/O 
  Benefits:
    allows for concise programs
    easier to parallelize
    much cleaner mathematically

Depending on your point of view, I/O in Haskell is either a big problem,
weird, or truly elegant.  Personally I rate it as elegant & weird.

In any case, even if you don't use Haskell in your work in the future,
it's useful to understand the functional programming style, which can
be an excellent way to write certain kinds of expressions in in other
languages as well (e.g. Python).  Also, understanding Haskell's type
system will give you insight into generic types in Java and other
object-oriented languages.

-}


{- SOME BUILT-IN TYPES:
  3 :: Integer
  1.0 :: Double
  'a' :: Char
  True :: Bool
  [1,2,3] :: [Integer]         -- lists
  [1.3,2.8] :: [Double]
  [True,True,True,False] :: [Bool]
  "fred" :: String (which is an alias for [Char]

  [True,3] -- gives a type error ... lists can contain only one type of element

-}

{- To apply a function to arguments just give the function name followed
   by the arguments (no parentheses needed)

sqrt 2
my_member 3 [1,2,3,4,5]
take 4 [2, 3, 5, 7, 11, 13, 17]

Infix operators are functions as well:
3+4

The usual precedence rules apply for operators:
3+4*5
3^2*100

Function application binds more tightly than infix operators:
sqrt 2*10

Case is significant.  Values and functions start with a lower-case
letter; types start with an upper-case letter.

-}

-- MORE ABOUT LISTS

{-

  [0,5 .. 100 ] -- the list of multiples of 5 between 0 and 100
  [1..] -- the infinite list of positive integers
  [1,3 ..] -- the infinite list of odd positive numbers

: is an infix 'cons' (list constructor) operator.  Example:

  3 : [4,5]

head gets the first element of a list
head [1,2,3]

tail gets the rest of the list
tail [1,2,3]

++ is the infix append operator:

[1,2] ++ [3,4,5]

length finds the length of a list

null tests whether a list is empty
null [1,2,3]
-}

{- STRINGS

strings are just lists of characters (type Char).  Try this:
['y', 'u', 'p']

The show function takes a value and turns it into a string:
show (3+4)
-}

{- PAIRS, TRIPLES, AND TUPLES

A pair is a pair of other values, and is written like this:

(1, 4)

Other examples:
(True, False)
(1, "squid")

Note that the types of the first and second element don't need to be the
same.  (Pairs always have two elements but the first and second element can
be of different types.  Lists can have any number of elements but they must
all have the same type.)

fst is a function to get the first element of a tuple;
snd gets the second.

We can also have triples:
 (1, 0, False)

'tuple' is the most general name -- this has 0 or more elements.  Note that
a tuple with 4 Booleans has a different type from a tuple with 3 Booleans!

But a list of Booleans always has type [Bool]


-}

-- DEFINING A FUNCTION

double :: Integer -> Integer
double x = 2*x

{- Haskell will infer the types of most things, but it's good form to
declare them (gives machine-checkable documentation).  Also the
inferred type might surprise you by being more general than expected.

For now, when we declare types, we will often use more restrictive types
than what Haskell allows.  (We're going to ease into Haskell's rich and
rather complex type system -- more on this later.)  It's always OK to
declare a more restrictive type than the one Haskell will infer, but you
get an error if you try to be more general; or of course if you give the
wrong type.

For 'triple' we haven't declare the type of the triple function.
The inferred type will allow this to work with floats as well as
integers and uses the type class mechanism (to be discussed later).
-}

triple x = 3*x


{- Haskell is case-sensitive ... function names should start with lower
   case, type names and constructors with upper case -}

-- SIMPLE RECURSION EXAMPLES

-- recursive factorial
rec_factorial :: Integer -> Integer
rec_factorial n =
    if n>0
      then n * rec_factorial (n-1)
      else 1

{- here's another version with error checking:

rec_factorial n =
    if n>0
      then n * rec_factorial (n-1)
      else if n==0 
        then 1
        else error "factorial called with n<0"

-}

{- White space is significant in parsing Haskell!
   There's some rule about how tabs are processed, but I avoid them and
   always use spaces. -}


-- factorial using the built-in prod function
-- note that this gives 1 for the case of n<0 rather than an error
factorial :: Integer -> Integer
factorial n = product [1..n]

-- type Integer can have an indefinite number of digits!
-- (there is also a type Int that is limited to hardware integers)

-- POLYMORPHIC TYPES (also note use of pattern matching in function def)

my_length        :: [a] -> Integer
my_length []     = 0
my_length (x:xs) = 1 + my_length xs

-- STYLE RECOMMENDATION: watch for opportunities to use pattern matching in
-- function definitions.  This often leads to cleaner definitions than
-- using if expressions or guards (discussed later).


-- MORE RECURSION EXAMPLES

-- sum the elements in a list (there is also a built-in function 'sum')
rec_sum []     = 0
rec_sum (x:xs) = x + rec_sum xs

-- append two lists (note that this is built-in as the operator ++)
append [] ys     = ys
append (x:xs) ys = x : append xs ys

-- CASE EXPRESSIONS

choose_creature x = 
    case x of 
      0 -> "squid"
      1 -> "octopus"
      2 -> "anemone"
      _ -> "unknown"

-- pattern matching for function definitions is equivalent to using case
-- expressions ... for example:

choose_another_creature 0 = "squid"
choose_another_creature 1 = "octopus"
choose_another_creature 2 = "anemone"
choose_another_creature _ = "unknown"

-- and conversely:

case_append xs ys =
    case xs of
      [] -> ys
      (z:zs) -> z : append zs ys

{- A useful function in the Prelude is zip, which takes two lists and
  returns a new list consisting of pairs, with the first element of
  the pair taken from the first list and the second element taken from
  the second list.  (This is useful in the Haskell project.)  For
  example try
     zip [1,2,3] [100,200,300]

What is its type? -}


-- CURRYING!  

plus :: Integer -> Integer -> Integer
plus x y = x+y

-- note that the type of plus is  Integer -> Integer -> Integer, 
-- NOT (Integer , Integer) -> Integer

-- we can apply plus to just one argument (check what the type is of incr)
incr = plus 1

{-- this last definition is also an example of Pointfree programming .... 
we don't mention the argument.  This is considered a cleaner definition than
incr a = plus 1 a
--}

{- HIGHER-ORDER FUNCTIONS

The Haskell library includes a map function.  This applies the function to
each element of a list and returns a list of the results.  Try evaluating 

   map incr [1,2,3]
-}

-- we can also easily define map ourselves:
my_map :: (a -> b) -> [a] -> [b]
my_map f [] = []
my_map f (a:x) = f a : my_map f x

-- try applying my_map to double, factorial, not, etc.

-- the types of the argument list and result list can be different
on_off :: Bool -> Integer
on_off False = 0
on_off True  = 1


-- now try my_map on_off [True,True,False]

-- ANONYMOUS FUNCTIONS

doubletrouble :: Integer -> Integer
doubletrouble = \x -> 2*x

-- try this:
--   my_map (\x -> x+5) [1,2,3]

{- FILTER
Another useful higher-order function is filter:

filter :: (a -> Bool) -> [a] -> [a]
filter even [1..20]

In the following expression we are using the isUpper function from the 
Data.Char module.

filter Data.Char.isUpper "Oliver Twist" 



Suppose we want function to filter out the elements in a list that aren't
divisible by 3

We could write it as:
   threes s = filter (\x -> mod x 3 ==0) s
But a more elegant definition is as follows.  (Why does this work?)
-}

threes = filter (\x -> mod x 3 ==0)

{- Ha!  Note that this is another example of Pointfree programming. -}

{- Function Composition

Another useful higher-order function is for doing function composition.
This is defined in the Prelude as the infix operator . and is often used
for pointfree programming.

Example: we defined two functions incr and double.

Now try evaluating : (double . incr) 10

We can also use function composition in a definition -} 

d_incr = double . incr

{- You could define the compose operator yourself:
compose f g x = f (g x)

What is the type of compose??  Try it and see what Haskell says for
  :t compose
or
  :t (.)


OK, the type of (.) is 

(.) :: (b -> c) -> (a -> b) -> a -> c

Notice that the two functions don't need to have  the same type -- 
rather, the argument type of the first has to be the return type of the 
second. 

Examples:

(const True) . incr

has type Integer -> Bool

first import Data.Char
so that the chr and ord functions are defined

then try

:t (chr . (+1) . ord) 
(chr . (+1) . ord) 'a'

-}


{- Static Scoping 

Illustrated with several versions of calculating the length of the
hypotenuse of a triangle - these all do the same thing) 
-}

hyp x y = 
    let xsq = x*x
        ysq = y*y
        sum = xsq + ysq
    in sqrt sum


hyp2 x y = 
    let sum = xsq + ysq
        xsq = x*x
        ysq = y*y
    in sqrt sum


-- let can be nested
hyp3 x y = 
    let xsq = x*x
        ysq = y*y
    in let sum = xsq + ysq
       in sqrt sum


-- where is another way to produce a local scope 
hyp4 x y =
    sqrt sum
    where sum = x*x + y*y

-- nested declarations shadow outer ones
shadow x =
    let x = 100
    in 2*x

{- Note that 'shadow' always returns 200, since the x in the let expression
   shadows the parameter.  This works just the same as in Scheme/Racket. -}

{- LIST COMPREHENSIONS -}

-- the list of squares of numbers between 0 and 10
squares100 = [x^2 | x <- [0..10]]

-- the infinite list of squares 
squares = [x^2 | x <- [0..]]


-- quicksort

qsort [] = []
qsort (a:x) = qsort [ b | b <- x, b<=a ] ++ [a] ++ qsort [ b | b <- x, b>a ]


-- all permutations of a list
perms [] = [[]]
perms x  = [ a:y | a <- x, y <- perms (remove a x) ]

remove a [] = []
remove a (x:y) | a==x       = y
               | otherwise  = x : remove a y


{- MORE HIGHER-ORDER FUNCTIONS -}

{- In many cases you can avoid writing a recursive function using
   higher-order functions such as map or foldr -}

my_member :: Eq a => a -> [a] -> Bool
my_member a x = or (map (==a) x)

-- This version of length uses the built-in const function,
-- which is defined as:
--      const k x = k
clever_length x = sum (map (const 1) x)
-- rewritten using $ to reduce the number of parens
clever_length2 x = sum $ map (const 1) x

-- or even more clever (Pointfree again).  Haskell fans would probably 
-- consider this as the best style.
very_clever_length = sum . map (const 1)

{- The foldr function ("fold right") takes three arguments: a
function, an initial value and a list.  For example, 
   foldr (+) 0 [1,8,5]
is equivalent to
    1+(8+(5+0))
which is 14.
-}

-- Using foldr we can elegantly define functions such as sum and product:
my_sum :: Num x => [x] -> x
my_sum = foldr (+) 0

my_product :: Num x => [x] -> x
my_product = foldr (*) 1

-- and similarly for functions to take the 'or' or 'and' of a list of booleans:
my_or :: [Bool] -> Bool
my_or = foldr (||) False
my_and :: [Bool] -> Bool
my_and = foldr (&&) True

{- Some details: usually we can omit type declarations and let Haskell
infer them, but in this case we need to include them due to an
annoying feature of Haskell's type system called the monomorphism
restriction.  More on this in TypesNotes.hs

All of the functions used above are commutative and
associative (the easier case to think about).  So in the example we
didn't really need the parentheses in

    1+(8+(5+0))

The parentheses do matter for non-associative functions.  For example:

    foldr (-) 0 [1,8,5]

is equivalent to

    1-(8-(5-0))

and not 1-8-5-0

There is another function foldl ("fold left") that does the opposite
bracketing.

    foldl (-) 0 [1,8,5]

is equivalent to

    ((0-1)-8)-5

foldr can work on infinite lists, while foldl cannot; but for finite
lists foldl will usually be more efficient.  (Since we generally won't
be concerned with relatively minor efficiencies in 341, and since the
instructor does have a strange fondness for infinite data structures,
we'll just use foldr.)  See the tutorial for more information.  
-}
