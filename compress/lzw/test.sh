#! /usr/bin/env bash
make
testinput="^WED^WE^WEE^WEB^WET"

echo "input: $testinput"

echo "test lzw compress process"
echo -n "$testinput" | ./com.out > out

echo "test lzw uncompress process"
./uncom.out out
