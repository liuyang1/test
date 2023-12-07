a_map (Just x) = Right x
a_map Nothing = Left ()

a_map_inv (Right x) = Just x
a_map_inv (Left ()) = Nothing

data Shape = Circle Float
           | Rect Float Float
           | Square Float

area (Circle r) = pi * r * r
area (Rect w h) = w * h
area (Square w) = w * w

circ (Circle r) = 2 * pi * r
circ (Rect w h) = 2 * (w + h)
circ (Square w) = 4 * w

e_map (True, x) = Left x
e_map (False, x) = Right x

e_map_inv (Left x) = (True, x)
e_map_inv (Right x) = (False, x)
