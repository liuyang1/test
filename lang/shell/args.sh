#! /usr/bin/env bash

echo "arg num: $#"
echo "arg #1: $1"
echo "arg #1: $2"

echo "$*"
echo "$@"
# one arg with whitespace, This style works.
for a in "$@"; do
    echo "$a"
done
# one arg with whitespace, This style break.
for a in $*; do
    echo "$a"
done
echo "$-"
