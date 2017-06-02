# How to treat linker's warnings as error?

## Background

We find some linker warnings sometimes. Compared with compiler warnings, this is a kind of potential risk issue, too. This kind issue is so hard to debug in runtime, as it may introduce wrong ASM instrucments.

Linker warnings inclues:

- linker different object files which compiled with different enum options (fixed length of variable length)

When linker show this warning, it crash when try to access NULL pointer. The C source code have no this error. It's hard to debug this issue.

## Solution

Treating linker's warnings as error, could fix this issue totally. Just add `--fatal-warnings` as linker's option.

test code check `Makefile` and `test.c`

## Limit

- old version's gcc may not support this option.
    - gcc 4.4.5 version don't support this option.
    - gcc 5.4.0 version support it.

## Ref

- gcc manual https://gcc.gnu.org/onlinedocs/
- sample code https://stackoverflow.com/questions/15977349/how-can-i-generate-a-simple-linker-warning-in-the-linux-kernel
