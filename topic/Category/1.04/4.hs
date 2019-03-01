 -- like Maybe, Nothing, Just
data M a = None | It a deriving (Show, Read)

root x
  | x < 0 = None
  | otherwise = It (sqrt x)

reciprocal x
  | x == 0 = None
  | otherwise = It (1/x)

isIt (It _) = True
isIt _ = False

fromIt (It x) = x

(>=>) f g x = let y = g x in if isIt y then f (fromIt y) else None

rootRecip = reciprocal >=> root

recipRoot = root >=> reciprocal

unit f = map f [4, 0, (-1)]

test = do
    print $ unit rootRecip
    print $ unit recipRoot

