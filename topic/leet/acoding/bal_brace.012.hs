bra x = "(" ++ x ++ ")"

balbra 0 = [""]
balbra n = [bra x ++ rest | m <- [0..n-1], x<-balbra m, rest <- balbra (n-m-1)]
