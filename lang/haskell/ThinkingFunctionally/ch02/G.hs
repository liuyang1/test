showDate (day, month, year) = showNth day ++ " " ++ showMonth month ++ ", " ++ show year

showNth n = show n ++ case n `mod` 10 of
                          1 -> "st"
                          2 -> "nd"
                          3 -> "rd"
                          _ -> "th"


showMonth x = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"] !! (x - 1)

main = do
        print $ showDate (1, 1, 2000)
        print $ showDate (10, 12, 2013)
        print $ showDate (21, 11, 2020)
