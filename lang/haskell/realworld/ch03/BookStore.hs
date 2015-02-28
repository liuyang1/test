-- file: ch03/BookStore.hs

-- data, declare a new struct?
--
-- Int: Identity
-- String: name of Book
-- [String]: [] optional for author
-- BookInfo is same with 3-tuple of type (Int, String, [String]),
-- but it has a distinct type
data BookInfo = Book Int String [String] deriving (Show)
