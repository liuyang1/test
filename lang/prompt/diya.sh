#!/usr/bin/env bash

######################################################################
# @author      : lyz
# @file        : diya
# @created     : Friday Sep 13, 2024 15:26:17 CST
#
# @description : 
######################################################################

function diyaless() {
	response=$(mcurl 'https://api.chat.com.amazon.dev' --data-raw '{"prompt":"'"$1"'","chatHistory":[]}' 2>/dev/null)
	completion=$(echo "$response" | jq -r '.completion')
	echo "$completion" | glow
}

# diya_history=""
# function diya() {
# 	data="{\"prompt\":\"$@\",\"chatHistory\":[$diya_history]}"
# 	echo $data
# 	response=$(mcurl 'https://api.chat.com.amazon.dev' --data-raw "$data" 2>/dev/null)
# 	completion=$(echo "$response" | jq -r '.completion')
# 	echo "$completion" | glow
# 	if [[ -n $diya_history ]]; then
# 		diya_history=$diya_history,
# 	fi
# 	diya_history=$diya_history"{\"sender\":\"Human\",\"text\":\"$@\"},{\"sender\":\"Assistant\",\"text\":$(echo -n $completion | jq -Rsa .)}"
# }
# 
# function clear_diya () {
# 	diya_history=""
# }
