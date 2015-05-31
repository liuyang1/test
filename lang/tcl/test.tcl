#! /usr/bin/expect -f
spawn bash test.sh
expect "first"
send "hello\n"
expect "second"
send "world\n"

# interact command with keep user process continue to run
# without this, user process will end immediatly
interact
