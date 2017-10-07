import LastDigit
import Test.Hspec
import Test.QuickCheck

h [] = 1
h [x] = x
h (x:y:xs)
    | null xs = x ^ y
    | otherwise = x ^ (h (y:xs))

naiveLastDigit xs = h xs `mod` 10

zeros = do
    it "should work for zeros" $ do
        lastDigit []         `shouldBe` 1
        lastDigit [0,0]      `shouldBe` 1 -- 0 ^ 0
        lastDigit [0,0,0]    `shouldBe` 0 -- 0^(0 ^ 0) = 0^1 = 0

aa = [[13, 3, 5]
     ,[13, 4, 6]
     ,[14, 7, 7]
     ,[17, 6, 3]
     ,[19, 4, 5]
     ,[7, 7, 7]
     ,[13, 3, 13]
     ,[15, 5, 5]
     ,[12, 18]
     -- ,[9, 9, 9] -- too large
     ]

examples = do
  it "should work for some examples" $ do
    lastDigit [1,2]      `shouldBe` 1
    lastDigit [6, 21]    `shouldBe` 6 -- 21936...856
    lastDigit [3,4,5]    `shouldBe` 1
    lastDigit [4,3,6]    `shouldBe` 4
    lastDigit [7,6,21]   `shouldBe` 1
    lastDigit [12,30,21] `shouldBe` 6
    lastDigit [937640,767456,981242] `shouldBe` 0
    lastDigit [123232,694022,140249] `shouldBe` 6
    lastDigit [499942,898102,846073] `shouldBe` 6
    map lastDigit aa `shouldBe` map naiveLastDigit aa

simpleProperties = do
  it "lastDigit [x] == x `mod` 10" $
    property (\(NonNegative x) -> lastDigit [x] `shouldBe` x `mod` 10)
  it "lastDigit [x, y] == x ^ y `mod` 10" $
    property (\(NonNegative x) (NonNegative y) -> lastDigit [x, y] `shouldBe` x ^ y `mod` 10)
  it "lastDigit [x, y, 2] == x ^ (y ^ 2) `mod` 10" $
    property (\(NonNegative x) (NonNegative y) -> lastDigit [x, y, 2] `shouldBe` x ^ (y ^ 2) `mod` 10)
  it "lastDigit [x, y, 3] == x ^ (y ^ 3) `mod` 10" $
    property (\(NonNegative x) (NonNegative y) -> lastDigit [x, y, 3] `shouldBe` x ^ (y ^ 3) `mod` 10)
  -- it "lastDigit [x, y, 4] == x ^ (y ^ 4) `mod` 10" $
  --   property (\(NonNegative x) (NonNegative y) -> lastDigit [x, y, 4] `shouldBe` x ^ (y ^ 4) `mod` 10)

main :: IO ()
main = hspec $ do
  describe "zeros" zeros
  describe "Examples" examples
  describe "Simple properties" simpleProperties

simple = do
    print $ lastDigit2 6 21 -- 16
    print $ lastDigit [6, 21] -- 6
    print $ lastDigit [7, 6, 21] -- 1
    print $ lastDigit [7, 13, 3] -- 7
    print $ lastDigit [7, 31, 3] -- 3
