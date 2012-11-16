#! /usr/bin/env bash

dirs=(  "test.g"
        "notes.g"
        "svcTSmux.g")

for d in ${dirs[@]}
do
    cd ~/git/
    echo $d
    cd $d

    #git add *
    #git add -u
    #git ci -m "daily"
    git push
#    pwd
done
