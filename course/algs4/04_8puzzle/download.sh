#! /usr/bin/env bash
set -e errexit

DIR="test"
FN="8puzzle-testing.zip"

[[ -d "$DIR" ]] || mkdir "$DIR"
pushd "$DIR"
wget http://coursera.cs.princeton.edu/algs4/testing/8puzzle-testing.zip
unzip "$FN"
rm -rf "$FN"
popd

echo "download $FN"
