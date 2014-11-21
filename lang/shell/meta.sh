#! /usr/bin/env bash
seq=(a b c)
a=1
b=2

for i in ${seq[@]}
do
    echo ${!i}
done
