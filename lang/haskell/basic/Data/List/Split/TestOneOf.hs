import Data.List.Split (oneOf, split)

a = split (oneOf ",;.! ") "  Hello, World! Yang, Liu"
