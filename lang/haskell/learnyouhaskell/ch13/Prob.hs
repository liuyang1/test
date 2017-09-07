import Data.List
import Data.Ratio
import Data.Function
import Control.Applicative
import Control.Arrow

newtype Prob a = Prob { getProb :: [(a,Rational)] } deriving Show

instance Functor Prob where
    fmap f (Prob xs) = Prob $ map (first f) xs

-- class (Functor f) => Applicative f where
--      pure :: a -> f a
--      (<*>) :: f (a -> b) -> f a -> f b
instance Applicative Prob where
    pure x = Prob [(x, 1 % 1)]
    (<*>) (Prob fs) (Prob xs) = Prob [(f x, p * q)|(f, p) <- fs, (x, q) <- xs]

flatten :: Prob (Prob a) -> Prob a
flatten (Prob xs) = Prob (concatMap multAll xs)
    where multAll (Prob innerxs,p) = map (Control.Arrow.second (p *)) innerxs

instance Monad Prob where
    return x = Prob [(x,1%1)]
    m >>= f = flatten (fmap f m)
    fail _ = Prob []

data Coin = Heads | Tails deriving (Show, Eq)

coin :: Prob Coin
coin = Prob [(Heads,1%2),(Tails,1%2)]

loadedCoin :: Prob Coin
loadedCoin = Prob [(Heads,1%10),(Tails,9%10)]

flipThree :: Prob Bool
flipThree = do
    a <- coin
    b <- coin
    c <- loadedCoin
    return (all (==Tails) [a,b,c])

groupSame (Prob xs) = Prob $ map sumProb grouped
    where sorted = sortOn fst xs
          grouped = groupBy ((==) `on` fst) sorted
          sumProb = foldl1 (\(a, p) (_, q) -> (a, p + q))

main = do
    print flipThree
    print $ groupSame flipThree
