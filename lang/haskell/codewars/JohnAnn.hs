annf 0 = 1
annf x = x - johnf (annf (x - 1))

johnf 0 = 0
johnf x = x - annf (johnf (x - 1))

john x = map johnf [0..x]
ann x = map annf [0..x]

sum_john = sum . john
sum_ann = sum . ann
