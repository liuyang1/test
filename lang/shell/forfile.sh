#! /usr/bin/env bash
src_path=~/obj/src
dst_path=~/obj/dst

for f in $src_path/*
do
	#translate src filename
	echo $f
	wf=`cygpath -w $f`
	base=`basename $wf`
	echo $base
	#translate dst filename
	df=$dst_path/$base.mp4
	wdf=`cygpath -w $df`
	echo $wdf

	suffix=`echo $base | cut -s -d '.' -f2`
	echo $suffix
	if [ $suffix = "mp4" ];then
		echo "skip"
	else
		echo "handle"
		ffmpeg -i $wf -f mp4 $wdf
	fi
done
