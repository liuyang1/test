-- file: ch03/BookStore.hs

-- data, declare a new struct?
--
-- Int: Identity
-- String: name of Book
-- [String]: author name list
-- BookInfo is same with 3-tuple of type (Int, String, [String]),
-- but it has a DISTINCT type
data BookInfo = Book Int String [String] deriving (Show)
-- BookInfo is type, for type construct
-- Book is value construct
-- they could be same, but also different.

myInfo = Book 1234 "Algebra of Programming" ["Richard Bird", "Oege de Moor"]

type CustomerID = Int
type ReviewBody = String

data BookReview = BookReview BookInfo CustomerID ReviewBody

type BookRecord = (BookInfo, BookReview)

-- algebraic data type
data Bool = False | True


type CardHolder = String
type CardNumber = String
type Address = [String]

data BillingInfo = CreditCard CardNumber CardHolder Address
                 | CashOnDelivery
                 | Invoice CustomerID
                   deriving (Show)
