#! /usr/bin/env bash
seq=(a b c)
a=1
b=2

for i in ${seq[@]}
do
    echo ${!i}
done

## result ##
# 1 ## expand ${!a} -> $a -> 1
# 2
#   ## expand ${!c} -> $c -> empty
