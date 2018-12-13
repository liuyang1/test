{- 

TYPES IN HASKELL 
CSE 341

Haskell has an exceptionally rich type system.

Some important features:
  - static typing
  - type inference
  - polymorphic functions
  - type classes

We've seen the basics already.  Here we'll look at some further aspects of
types in Haskell.

-}

module TypesNotes
    where


-- SIMPLE USER-DEFINED TYPES

data SimpleColor = Red1 | Green1 | Blue1

{- Trying to interact with this is going to get annoying, since
   SimpleColors don't know how to print themselves.  Let's fix this by
   automatically defining show and read functions using the 'deriving' 
   construct. -}

data Color = Red | Green | Blue
             deriving (Show,Read)

{- Now colors know how to show themselves; you can also read in color
   values from strings.  Show and Read are type classes -- later we
   demonstrate how to do this from scratch instead, without the
   "deriving" construct . -}


{- TYPE CLASSES AND OVERLOADING -}

{- parametric polymorphism vs overloading.

A polymorphic type includes a type variable (the parameter, hence the
name parametric).

For a polymorphic function, there is just a single definition, and the 
function can be applied to different types.  Examples: 
  append :: [a] -> [a] -> [a]
  const :: a -> b -> a

In languages such as Pascal or Ada, + is overloaded.  This means that there
are several different versions of the + operator; which one is used depends
on the types of the operands.

Type classes allow this to be handled cleanly in Haskell.

The Prelude includes the following type class:

class Eq a where
    (==) :: a -> a -> Bool

This says that the Eq class has a == operation with the given type.

Note that we don't want to be able to use the == operation on all types!
(It's not decidable in general whether two functions are equal.)  Type
classes support this.


Then the Prelude goes on to use this class:

instance Eq  Integer where 
  x == y    = primEqInteger x y

instance Eq  Float where 
  x == y    = primEqFloat x y


== is overloaded.

As an example of using Eq, here is the type of the member function:

my_member :: Eq a => a -> [a] -> Bool

This says that the type of my_member is:
    a -> [a] -> Bool 
for any type a in the type class Eq.

We can extend a type class (i.e. define a subclass):

class (Eq a) => Ord a where
    compare                :: a -> a -> Ordering
    (<), (<=), (>=), (>)   :: a -> a -> Bool
    max, min               :: a -> a -> a

As an example of using Ord, the type of smember (member for sorted lists) is:

smember :: Ord a => a -> [a] -> Bool


Multiple inheritance for type classes is allowed.

The built-in numeric classes include:
   Int (fixed precision integers)
   Integer (infinite precision integers)
   Float
   Double

Now try finding the types of:
  3
  3.5
  (1, 3, True, 'x')

-}


{- POLYMORPHIC USER DEFINED TYPES -}

-- Let's define a Tree type.  We will have an EmptyTree (with no
-- value) for the leaves, and associate a value with each interior
-- node.  Note that this is a recursive type!

-- We will automatically define show and read functions using
-- the 'deriving' construct.

data Tree a = EmptyTree | Node a (Tree a) (Tree a)
              deriving (Show,Read)

small :: Tree Integer
small  = (Node 4 EmptyTree (Node 5 EmptyTree EmptyTree))

big :: Tree Integer
big  = (Node 10 small (Node 20 EmptyTree small))

-- do a preorder traversal and return the result as a list
preorder :: Tree a -> [a]
preorder EmptyTree = []
preorder (Node x left right) = [x] ++ preorder left ++ preorder right

tree_sum :: Tree Integer -> Integer
tree_sum EmptyTree = 0
tree_sum (Node x left right) = x + tree_sum left + tree_sum right


-- let's try the same function, but let Haskell infer the type:
tsum EmptyTree = 0
tsum (Node x left right) = x + tsum left + tsum right


{- Using infinite data structures with user-defined types -}

verylargetree :: Tree Integer
verylargetree = Node 4 verylargetree verylargetree

make_tree :: Integer -> Tree Integer
make_tree n = Node n (make_tree (n+1)) (make_tree (2*n))

{- to deal with infinite trees, let's define a function tree_take that
   finds just the first n levels of a tree (analogous to 'take' for lists) -}

tree_take :: Integer -> Tree a -> Tree a
tree_take 0 t = EmptyTree
tree_take n EmptyTree = EmptyTree
tree_take n (Node x left right) = 
    Node x  (tree_take (n-1) left) (tree_take (n-1) right)

{- The Show Class 

Types that are members of the Show class have functions that convert
instances of those types into strings.  We'll just be using one of those,
the 'show' function.

For example, try these:

show 5
show True
show sqrt
show small

Suppose that we don't want to use the default version of 'show' that gets
built when you use the 'deriving' construct.  Here's an example:

-}

data Point = Point Integer Integer

instance Show Point where
    show (Point x y) = show x ++ "@" ++ show y

{- We could use this also for the Color example in place of using
  "deriving".  This is just to illustrate what is going on -- using
  "deriving" is both simpler and better style! -}

data ComplicatedColor = Red2 | Green2 | Blue2

instance Show ComplicatedColor where
    show Red2 = "Red2"
    show Green2 = "Green2"
    show Blue2 = "Blue2"


{- The Monomorphism Restriction

The "monomorphism restriction" is an annoying feature of Haskell's
type rules.  If you don't have a explicit type signature, the rule may
give specific types to free variables using "type defaulting" rules.
This can be annoying if you have some polymorphic expression for which
Haskell infers a less general type, and you get type errors.

Here's an example.  If you define my_sum without specifying an
explicit type, like this:

my_sum = foldr (+) 0

the compiler will give a scary-looking error.  You can get around this
by adding an explicit type:

my_sum :: Num x => [x] -> x
my_sum = foldr (+) 0

Or you can turn off the monomorphism restriction with a language
pragma at the beginning of your program:

{-# LANGUAGE NoMonomorphismRestriction #-}

See these wiki pages for more information:
https://wiki.haskell.org/Monomorphism_restriction 
https://wiki.haskell.org/Language_Pragmas

-}
