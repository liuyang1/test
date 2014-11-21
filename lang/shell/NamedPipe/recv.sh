#! /usr/bin/env bash
while :
do
    read msg
    if [ "$msg" == "" ]; then
        continue
    else
        echo "recv $msg" >> testfile
    fi
done < fifoT
