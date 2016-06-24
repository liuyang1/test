import Data.Maybe
import Parser
import Data.Function
import Data.Char
import Data.Bits
import Data.List
import qualified Data.Map.Strict as M
import qualified Data.ByteString as B
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

-- Key is "Haskell Is Great!"

--------------------------------------------------------------------------------
-- Vigenere chipher

(.:) g f a b = g (f a b)

char2Int c = ord c - (ord 'a')
int2Char i = chr (i + (ord 'a'))

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

-- decryptWithKey :: B.ByteString -> FilePath -> IO ()
decryptWithKey key fn = do
    a <- B.readFile fn
    let txt = xorDec key $ bs2Str a
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
getBadTs lstf tsf = do
    lst <- parseFile lstf :: IO (Maybe [TId])
    ts <- parseFile tsf :: IO (Maybe [Transaction])
    let ret = filter (\t -> tid t `elem` (fromJust lst)) (fromJust ts)
     in return ret

-- insert (from t) (amount t) $ 

incM = M.insertWith (+)
updateT t m = incM (from t) (- (amount t)) $ incM (to t) (amount t) m

getFlow = foldr updateT M.empty

getCriminal :: M.Map String Integer -> String
getCriminal = fst . maximumBy (compare `on` snd) . M.toList

-- "Shaanan Cohney"


