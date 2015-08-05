import Data.Char
import Data.Array
import Data.List
import Data.Maybe

fdindexh (']':xs) csr 1 = csr
fdindexh (']':xs) csr acc = fdindexh xs (csr + 1) (acc - 1)
fdindexh ('[':xs) csr acc = fdindexh xs (csr + 1) (acc + 1)
fdindexh (x:xs) csr acc = fdindexh xs (csr + 1) acc
fdindex s p = p + fdindexh (drop p s) 0 0

bkindexh ('[':xs) csr 0 = csr
bkindexh ('[':xs) csr acc = bkindexh xs (csr + 1) (acc - 1)
bkindexh (']':xs) csr acc = bkindexh xs (csr + 1) (acc + 1)
bkindexh (x:xs) csr acc = bkindexh xs (csr + 1) acc
bkindex s p = p - bkindexh (reverse $ take p s) 0 0

setStr [] _ v = [v]
setStr s@(s0:st) 0 v = v: st
setStr (s0:st) p v = s0: setStr st (p - 1) v
  -- | ins == '+' = run dp (setStr ds dp (chr ((dd + 1) `mod` 255))) ipn is s
  -- | ins == '-' = run dp (setStr ds dp (chr ((dd - 1) `mod` 255))) ipn is s
  -- | ins == ',' = run dp (setStr ds dp s0) ipn is st

run :: (Num i, Ix i) => i -> Array i Char -> Int -> String -> String -> String
run dp ds ip is s
  | ip >= length is = []
  | ins == '>' = run (dp + 1) ds ipn is s
  | ins == '<' = run (dp - 1) ds ipn is s
  | ins == '.' = chr dd: run dp ds ipn is s
  | ins == '+' = run dp (ds//[(dp, chr ((dd + 1) `mod` 255))]) ipn is s
  | ins == '-' = run dp (ds//[(dp, chr ((dd - 1) `mod` 255))]) ipn is s
  | ins == ',' = run dp (ds//[(dp, head s)]) ipn is (tail s)
  | ins == '[' && dd /= 0 = run dp ds (fdindex is ip) is s
  | ins == ']' && dd /= 0 = run dp ds (bkindex is ip) is s
  | otherwise =  run dp ds ipn is s
    where ins = is !! ip
          ipn = ip + 1
          dd = ord $ ds ! dp


-- output "A"
aBF = "+++++ +++[>+++++ +++<-]>+."
-- output "Hello World!"
helloBF = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+."
-- "1, 1, 2, 3, 5, 8, 13, 21, 34, 55"
numbersBF = ",>+>>>>++++++++++++++++++++++++++++++++++++++++++++>++++++++++++++++++++++++++++++++<<<<<<[>[>>>>>>+>+<<<<<<<-]>>>>>>>[<<<<<<<+>>>>>>>-]<[>++++++++++[-<-[>>+>+<<<-]>>>[<<<+>>>-]+<[>[-]<[-]]>[<<[>>>+<<<-]>>[-]]<<]>>>[>>+>+<<<-]>>>[<<<+>>>-]+<[>[-]<[-]]>[<<+>>[-]]<<<<<<<]>>>>>[++++++++++++++++++++++++++++++++++++++++++++++++.[-]]++++++++++<[->-<]>++++++++++++++++++++++++++++++++++++++++++++++++.[-]<<<<<<<<<<<<[>>>+>+<<<<-]>>>>[<<<<+>>>>-]<-[>>.>.<<<[-]]<<[>>+>+<<<-]>>>[<<<+>>>-]<<[<+>-]>[<+>-]<<<-]"
-- eofBF = ",[.,]"
eofBF = "+[,.]"
eofString = "testinput" ++ [chr 0]
-- ret = bf eofBF eofString
outnBF = ">" ++ (replicate (ord '!') '+') ++ "<,[>.<-]"
outnRet = bf outnBF [chr 3]

hs = numbersBF
al = 10240
a = array (0, al) [(i, chr 0) | i <- [0..al]]
bf = run 0 a 0
ret = bf hs [chr 10]
