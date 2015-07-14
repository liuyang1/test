#! /usr/bin/env bash
gcc -g testgdb.c
gdb < cmd.gdb a.out
