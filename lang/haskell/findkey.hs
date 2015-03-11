findKey :: (Eq k) => k -> [(k,v)] -> Maybe v
findKey key = foldr (\(k,v) aac -> if key == k then Just v else aac) Nothing
