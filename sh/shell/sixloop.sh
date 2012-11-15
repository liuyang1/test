#! /usr/bin/env bash
sed -n "s/\(.\)\(.\)\(.\)\3\2\1/(\1\2\3-\3\2\1)/p" /usr/share/dict/words
