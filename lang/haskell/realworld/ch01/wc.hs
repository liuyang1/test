-- file: ch01/WC.hs
-- lines begin with "--" are comments

-- get input
-- split it to line
-- get length of list
-- print it
main = interact wordCount
    where wordCount input = show (length (lines input)) ++ "\n"

-- replace lines -> words
-- to count word number

-- replace (lines input) -> input
-- to count char number
