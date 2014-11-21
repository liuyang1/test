#! /bin/bash
MYDATE=`date +%y%m%d%H%M%S`
FILE=sm.$MYDATE.pkt
tcpdump tcp port 554 -i eth0 -s 0 -w $FILE
