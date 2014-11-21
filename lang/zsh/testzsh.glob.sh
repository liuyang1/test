#! /usr/bin/env bash
# glob * epxand problem

patterns=("/usr/*/lib/python*" "/usr/*/pythonabc*")
for p in $patterns
do
    echo "dir" $p
    for f in `ls "$p" 2>/dev/null`
    do 
        echo "including $f"
    done
done
