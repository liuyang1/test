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

parent and child may run concurrently (e.g., on different processors).

## what does wait() do? waits for any child to exit what if child exits before parent calls wait?

wait() - wait for a child process to stop or terminate

if child exits before parent calls wait, wait() could return immediately.

for multiple child processes, the order in which their status is reported is unspecified.

## how to execute echo?

by `execv` sys-call to fork one child-process to execute it.

## how to do I/O redirection (i.e., >)

as child-process is forked from parent process by `execv` sys-call, it have same status with parent-process. so we could change IO redirection in parent process.

pros::

- child-process change nothing
- parent-process's change is for every command

# lec1 homework

## shell

### Why need close unused pipe end?

- http://stackoverflow.com/questions/11599462/what-happens-if-a-son-process-wont-close-the-pipe-from-writing-while-reading
- http://stackoverflow.com/questions/19265191/why-should-you-close-a-pipe-in-linux

## xv6 boot

### cannot do anything after move mouse to `qemu` window

qemu take over all mouse and keyboard, so nothing reponse. just press `Ctrl + Alt`, then mouse come back

### gdb run result is different with the xv6-boot html page?

new gdb disable auto load from `.gdbinit`, need config it by `add-auto-load-safe-path [pwd]` or `set auto-load safe-path /` to disable this feature.

### explain result of `x/24x $esp` from gdb

check hw1.txt file
