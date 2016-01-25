type Birds = Int
type Pole = (Birds, Birds)

landLeft :: Birds -> Pole -> Pole
landLeft n (left, right) = (left + n, right)

landRight :: Birds -> Pole -> Pole
landRight n (left, right) = (left, right + n)

x -: f = f x
-- "Method Chaining" style is so easily made in Haskell
ret0 = (0, 0) -: landLeft 1 -: landRight 1 -: landLeft 2

main = do print ret0
