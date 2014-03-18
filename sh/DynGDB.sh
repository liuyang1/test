#! /usr/bin/env bash
GDB_READY=0
while [ "==" ]
do
	if [ "$GDB_READY" = "0" ];then
		export FORCE_REFRESH_ENV=1
		sleep 1
		export -n FORCE_REFRESH_ENV
		echo "waiting"+$GDB_READY
	else 
		if [ "$GDB_READY" = "1" ];then
			./vlc
			GDB_READY=2
		else
			sleep 1
		fi
	fi
done
