import Data.Char

type Write a = (a, String)

-- prone code with Writer Monad
m1 >=> m2 = \x ->
    let (y, s1) = m1 x
        (z, s2) = m2 y
        in (z, s1 ++ s2)

return x = (x, "")

pickEven xs = (filter (\x->length x `mod` 2 == 0) xs, "pickEven ")
upCase s = (map toUpper s, "upCase ")
toWords s = (words s, "toWords ")
unWords xs = (concat xs, "unWords ")

process = upCase >=> toWords >=> pickEven >=> unWords

main = do
    print $ process "abc defg hello world!"
