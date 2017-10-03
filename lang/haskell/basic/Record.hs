findKey :: (Eq k) => k -> [(k, v)] -> Maybe v
findKey key = foldr (\(k, v) acc -> if key == k then Just v else acc) Nothing

boolToNum True = 1
boolToNum False = 0

findKeyTimes key = foldr (\(k, _) acc -> acc + boolToNum (key == k)) 0
