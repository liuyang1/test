module Main where

import Data.Maybe
import Parser
import Data.Function
import Data.Char
import Data.Bits
import Data.List
import qualified Data.Map.Strict as M
import qualified Data.ByteString as B
import qualified Data.ByteString.Char8 as C
import qualified Data.ByteString.Lazy as L
import qualified Data.ByteString.Lazy.Internal as LI


--------------------------------------------------------------------------------
-- Exercise 1
filterSecrete a b = filter (/= 0) $ zipWith xor a b

getSecret :: FilePath -> FilePath -> IO B.ByteString
getSecret fa fb = do
    a <- B.readFile fa
    b <- B.readFile fb
    return $ B.pack $ filterSecrete (B.unpack a) (B.unpack b)

-- getSecret "dog.jpg" "dog-original.jpg"
-- Key is "Haskell Is Great!"

--------------------------------------------------------------------------------
-- Vigenere chipher

(.:) g f a b = g (f a b)

char2Int c = ord c - ord 'a'
int2Char i = chr (i + ord 'a')

addMod26 = (`mod` 26) .: (+)
minusMod26 = (`mod` 26) .: (-)

wrap f = int2Char .: f `on` char2Int

addChar = wrap addMod26
minusChar = wrap minusMod26

evalCrypt f key = zipWith f (cycle key)

vigenEnc = evalCrypt addChar
vigenDec = evalCrypt (flip minusChar)

--------------------------------------------------------------------------------
-- Exercise 2

xorDec = evalCrypt (chr .: xor `on` ord)

-- stupid way to remove .enc for filename
decFp = reverse . drop 4 . reverse

bs2Str = map (chr . fromEnum) . B.unpack

decryptWithKey :: B.ByteString -> FilePath -> IO ()
decryptWithKey key fn = do
    a <- B.readFile fn
    let txt = xorDec (C.unpack key) $ bs2Str a
        wf = decFp fn
    writeFile wf txt

-- decryptWithKey "Haskeel Is Great!" "victims.json.enc"

--------------------------------------------------------------------------------
-- Exercise 3

parseFile :: FromJSON a => FilePath -> IO (Maybe a)
parseFile fn = do
    a <- L.readFile fn
    let t = decode a
        in return t

-- parseFile "victims.json" :: IO (Maybe [TId])


--------------------------------------------------------------------------------
-- Exercise 4
getBadTs :: FilePath -> FilePath -> IO [Transaction]
getBadTs lstf tsf = do
    lst <- parseFile lstf :: IO (Maybe [TId])
    ts <- parseFile tsf :: IO (Maybe [Transaction])
    let ret = filter (\t -> tid t `elem` fromJust lst) (fromJust ts)
     in return ret

-- getBadTs "victims.json" "transactions.json"

--------------------------------------------------------------------------------
-- Exercise 5
incM = M.insertWith (+)
updateT t m = incM (from t) (- (amount t)) $ incM (to t) (amount t) m

-- | getFlow
-- input: transaction list
-- output: Map (name, amount)
getFlow :: [Transaction] -> M.Map String Integer
getFlow = M.filter (/= 0) . foldr updateT M.empty

--------------------------------------------------------------------------------
-- Exercise 6
getCriminal :: M.Map String Integer -> String
getCriminal = fst . maximumBy (compare `on` snd) . M.toList

-- "Shaanan Cohney"

test6 = do
    ts <- getBadTs "victims.json" "transactions.json"
    let crim = getCriminal $ getFlow ts
     in return crim

--------------------------------------------------------------------------------
-- Exercise 7
sortOn f = sortBy (compare `on` f)

splitPay :: M.Map String Integer -> ([(String, Integer)], [(String, Integer)])
splitPay ts = (pr, ps)
    where (payers, payees) = partition (\x -> snd x > 0) $ M.toList ts
          pr = reverse $ sortOn snd payers
          ps = sortOn snd payees

-- why not use fold, as pr, ps list is changing when iterate
makeTs [] [] _ ts = ts
makeTs ((nr,ar):pr) ((ns,as):ps) (t:tids) ts =
    let d = ar + as
     in if d > 0 then makeTs (insert (nr, d) pr) ps tids (appendT (-as))
                 else if d == 0 then makeTs pr ps tids (appendT ar)
                 else makeTs pr (insert (ns, d) ps) tids (appendT ar)
                     where appendT v = Transaction nr ns v t: ts

undoTs :: M.Map String Integer -> [TId] -> [Transaction]
undoTs fs tids = reverse $ makeTs payers payees tids []
    where (payers, payees) = splitPay fs

test7 = do
    ts <- getBadTs "victims.json" "transactions.json"
    let r = undoTs (getFlow ts) (repeat "")
     in return r

--------------------------------------------------------------------------------
-- Exercise 8
writeJSON :: ToJSON a => FilePath -> a -> IO ()
writeJSON fp a = L.writeFile fp (encode $ toJSON a)

test8 = do
    ts <- getBadTs "victims.json" "transactions.json"
    let r = undoTs (getFlow ts) (repeat "")
     in writeJSON "new-transaction.json" r

--------------------------------------------------------------------------------
-- Exercise 9
hw05 :: IO ()
hw05 = do
    scrt <- getSecret "dog.jpg" "dog-original.jpg"
    vict <- decryptWithKey scrt "victims.json.enc" -- generate victims.json
    trans <- getBadTs (decFp "victims.json.enc") "transactions.json"
    let ts = getFlow trans
        crim = getCriminal ts
        r = undoTs ts (repeat "")
     in do
         putStrLn crim
         writeJSON "new-transaction.json" r
