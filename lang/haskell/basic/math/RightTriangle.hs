-- demo for list comprehension
rightTriangles m = [(a, b, c) | c <- [1..m], b <- [1..c], a <- [1..b], a^2 + b^2 == c^2 ]

main = print $ rightTriangles 20
