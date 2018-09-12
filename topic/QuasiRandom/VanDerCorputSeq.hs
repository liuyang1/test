module VanDerCorputSeq where

-- 低差异分布
-- ref: 范德科皮特序列
-- https://zh.wikipedia.org/wiki/%E8%8C%83%E5%BE%B7%E7%A7%91%E7%9A%AE%E7%89%B9%E5%BA%8F%E5%88%97
-- https://zhuanlan.zhihu.com/p/20197323
-- https://web.archive.org/web/20120621193817/http://www.puc-rio.br/marco.ind/quasi_mc.html
-- http://web.maths.unsw.edu.au/~fkuo/sobol/


radical b = reverse . go b
    where go _ 0 = []
          go b x = r: go b q
              where (q, r) = x `divMod` b

bak b = go b . reverse
    where go b xs = sum $ zipWith (\a e -> a * b ^ e) xs [0..]

-- radical inversion
convertB b x = (bak b $ reverse xs, b ^ length xs)
    where xs = radical b x

vanDerCorputSeq b = map (convertB b) [0..]

seq2 = take 100 $ vanDerCorputSeq 2
seq3 = take 100 $ vanDerCorputSeq 3
seq10 = take 100 $ vanDerCorputSeq 10

halton2 = zip (vanDerCorputSeq 2) (vanDerCorputSeq 3)
halton3 = zip3 (vanDerCorputSeq 2) (vanDerCorputSeq 3) (vanDerCorputSeq 5)

hammersely2 n = zip (zip [0..] (repeat n)) (vanDerCorputSeq 2)
hammersely3 n = zip3 (zip [0..] (repeat n))
                     (vanDerCorputSeq 2)
                     (vanDerCorputSeq 3)

seqH3 = take 1024 $ hammersely3 1024

-- TODO: need differenct matrix
sobol = zip (vanDerCorputSeq 2) (vanDerCorputSeq 2)
