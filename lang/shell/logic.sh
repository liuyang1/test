#! /usr/bin/env bash
key="aaa"

# if [ "$key" == "aaa" ] || [ "$key" == "bbb" ]; then
if [ $key == "aaa" ] || [ "$key" == "bbb" ]; then
    echo "if clause"
else
    echo "else clause"
fi
