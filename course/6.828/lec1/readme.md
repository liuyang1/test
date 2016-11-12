# lec1

## system call
- what system calls does "ls" call?
    check `ls.strace` file for detail

## echo

- what are file descriptors? (0, 1, 2, etc. in read/write) echo.c
    0: stdin, 1: stdout, 2: stderr
    TODO:
- what if want to redirect i/o to a file? echo.c
    This file redirect by itself. close stdout, then open other file.
    file descriptors could be REUSED after last one is closed.

## Simpfied xv6 sh.c

## what does fork() do?
copies user memory copies process kernel state (e.g. user id) child gets a different PID child state contains parent PID returns twice, with different values
