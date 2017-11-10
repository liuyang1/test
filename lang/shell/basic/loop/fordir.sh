#! /usr/bin/env bash
dir="."
for f in $dir/* ; do
    if [[ -d "$f" ]]; then
        echo "$f"
        mkdir -p "$f/test"
    fi
done
