import Data.Function
import Data.List
import Control.Applicative

-- | use Applicative to combine function
partlist :: [String] -> [(String, String)]
partlist = (zip `on` (map unwords . tail . init)) <$> inits <*> tails
