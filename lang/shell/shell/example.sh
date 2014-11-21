#! /usr/bin/env bash
set -v
echo -n "input Y/n? "
read ans
ans=`echo $ans | tr [a-z] [A-Z]`
if [ $ans = Y ]
then
	echo "yes"
else
	echo "no"
fi
