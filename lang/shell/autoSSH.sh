#! /usr/bin/env bash

nossh_cb() {
    # add your code here
    return
}
while [ "==" ]
do
    ssh_d_process_num=$(ps aux | grep -E 'ssh \-' | grep -v grep | wc -l)
    echo "$ssh_d_process_num"
    if [ "$ssh_d_process_num" == "0" ];then
        nossh_cb
        echo "not ssh"
    else
        echo "ssh linking"
    fi
    sleep 10
done
