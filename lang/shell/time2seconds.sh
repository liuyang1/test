#!/usr/bin/env sh

######################################################################
# @author      : lyz
# @file        : 1
# @created     : Thursday Sep 05, 2024 17:48:32 CST
#
# @description : 
######################################################################


# echo "00:00:30" | tee | date +%s -d -
# date -d "00:12:30" +%s
time_str="23:45:12"
seconds=$(printf $time_str | awk -F: '{print (($1*3600)+($2*60)+$3)*0.06430}')
echo $seconds
