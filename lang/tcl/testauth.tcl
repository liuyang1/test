#! /bin/expect.exe -f
set user	root
set host	10.0.185.235
set timetout	10
spawn ssh $user@$host
expect "]*"

send "date >> testfile\r"

send "cat testfile\r"

send "exit\r"
