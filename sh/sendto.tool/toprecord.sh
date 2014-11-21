#!/bin/sh
top -d 1 | tee -a  toprecord.log
cat toprecord.log | grep 'COMMAND'|uniq  > top.log
cat toprecord.log | grep 'java' >>top.log
rm toprecord.log
