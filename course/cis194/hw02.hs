{-# OPTIONS_GHC -Wall #-}
module LogAnalysis where
import Log

parseMessage :: String -> LogMessage
parseMessage s = case splitAt 2 s of
                   ("I ", t) -> LogMessage Info n (drop 1 msg)
                       where [(n, msg)] = reads t::[(Int, String)]
                   ("W ", t) -> LogMessage Warning n (drop 1 msg)
                       where [(n, msg)] = reads t::[(Int, String)]
                   ("E ", t) -> LogMessage (Error no) n (drop 1 msg)
                       where [(no, tt)] = reads t::[(Int, String)]
                             [(n, msg)] = reads (drop 1 tt)::[(Int, String)]
                   _ -> Unknown s

parse :: String -> [LogMessage]
parse = map parseMessage . lines

insert :: LogMessage -> MessageTree -> MessageTree
insert msg Leaf = Node Leaf msg Leaf
insert msg@(LogMessage _ ts _) (Node left msg0@(LogMessage _ ts0 _) right)
  | ts < ts0 = Node (insert msg left) msg0 right
  | ts >= ts0 = Node left msg0 (insert msg right)
insert _ t = t

build :: [LogMessage] -> MessageTree
build = foldl (flip insert) Leaf

inOrder :: MessageTree -> [LogMessage]
inOrder Leaf = []
inOrder (Node left msg0 right) = inOrder left ++ [msg0] ++ inOrder right

isWrong :: LogMessage -> Bool
isWrong (LogMessage (Error no) _ _)
  | no >= 50 = True
  | otherwise = False
isWrong _ = False

whatWentWrong :: [LogMessage] -> [String]
whatWentWrong = map (\(LogMessage _ _ s) -> s ) . filter isWrong . inOrder . build
