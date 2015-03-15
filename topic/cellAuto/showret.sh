#! /usr/bin/env bash
d="."
for f in "$d"/* ; do
    if [[ "$f" != *".ret" ]]; then
        continue
    fi
    echo "$f"
    set -o errexit
    cat "$f" | head -n $(($LINES - 8))
    read
done
