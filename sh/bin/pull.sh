#! /usr/bin/env bash

dirs=(  "test.g"
        "notes.g"
        "svcTSmux.g")

for d in ${dirs[@]}
do
    cd ~/git/
    echo $d
    cd $d

    git pull origin master

    #$pwd
done
