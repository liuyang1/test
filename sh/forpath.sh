#! /usr/bin/env bash
dirs=("1" "b" "c")

for d in ${dirs[@]}
do
	echo $d
	cd $d

	scp ./* 
done

