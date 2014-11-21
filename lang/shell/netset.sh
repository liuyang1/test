#! /usr/bin/env bash

setMultiIf() {
# args ethX addr gateway
	eth=$1
	addr=$2
	table=${1}"table"
	ifconfig $eth $addr
	ip route add default dev $eth src $addr table $table
	ip rule add from $addr/32 table $table
	echo "setting $eth $addr ok"
}

setMultiIf eth2 192.168.1.20
setMultiIf eth3 192.168.1.70

ip route flush cache
