a_map (Just x) = Right x
a_map Nothing = Left ()

a_map_inv (Right x) = Just x
a_map_inv (Left ()) = Nothing
