#! /usr/bin/env bash
test=1
PORT=80
while [ "==" ]
do
	portnum=`netstat -aon | grep ":$PORT " | wc -l`
	echo $portnum
	if [ "$portnum" -gt "0" ];then
		process=`sudo netstat -aonp | grep ":$PORT " | awk '{print $7}'`
		pid=`dirname $process`
		echo $pid
		if [ "$test" -gt "0" ];then
			kill -9 $pid
		fi
	else
		break
	fi
	if [ "$test" -gt "0" ];then
		sleep 1
	fi
done
echo "clean OK"

