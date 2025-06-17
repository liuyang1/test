#!/usr/bin/env sh

######################################################################
# @author      : lyz
# @file        : combine_logic
# @created     : Saturday Dec 23, 2023 13:55:19 CST
#
# @description : test how to combine logic expression in bash/zsh
######################################################################


# true, always return zero
# false, always return non-zero

function logic_express() {
    echo $1 $2
    # ( eval $1 ) && ( eval $2 ) || ( eval $3 )
    # ( eval $1 ) && ( eval $2 )

    while true; do
        eval $1
        eval $2 || break
    done
}

logic_express 'true && echo "cmd1"' 'echo cmd2; false ' true
