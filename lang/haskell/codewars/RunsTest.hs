-- 游程检验（Runs test）
-- 又称连贯检验，根据样本标志表现所形成地游程地多少进行判断地检验方法
module RunsTest where

import Data.List

runsTest :: String -> Int
runsTest = length . group
