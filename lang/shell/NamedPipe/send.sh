#! /usr/bin/env bash
if [[ ! -p fifoT ]]; then
    mknod fifoT p
fi
pid=$(ps aux | grep recv.sh | grep -v grep)
echo "$pid"
echo "pid done"
if [[ "$pid" == "" ]]; then
    echo "start recv.sh"
    bash ./recv.sh &
fi
while :
do
    read name
    echo "$name"
    # date > fifoT
done > fifoT
