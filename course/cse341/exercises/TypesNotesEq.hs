{- 

TYPES IN HASKELL - CSE 341
Making user-defined types instances of the Eq type class

-}

module TypesNotesEq
    where

{- If we take the Color type from the TypesNotes.hs file, and try for
example to test whether a particular color is a member of a list of
colors, we will get an error, since testing for membership uses the ==
function and Color wasn't in the Eq type class.

We can fix this by adding Eq to the 'deriving' part of the definition:
-}

data Color = Red | Green | Blue
             deriving (Show,Read,Eq)

{- now this works:
Green `elem` [Red,Blue,Green]
-}

{- we can do the same thing with Tree: -}

data Tree a = EmptyTree | Node a (Tree a) (Tree a)
              deriving (Show,Read,Eq)

small :: Tree Integer
small  = (Node 4 EmptyTree (Node 5 EmptyTree EmptyTree))

big :: Tree Integer
big  = (Node 10 small (Node 20 EmptyTree small))

{- now this works:

small == small  -- evaluates to True 
small == big    -- evaluates to False 

You can also define your own == function, just as with defining
your own show function, as in the Point example.

See http://www.haskell.org/onlinereport/derived.html for other
possibilities for 'deriving'.

Incidentally, note that if we use the above 'deriving' clause for
Tree, 'a' (the type of item in the tree) also has to be in the Show,
Read, and Eq type classes.  So we can't make a tree of functions.

-}
