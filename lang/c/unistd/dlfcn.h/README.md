# dlfcn

file              | comment                   |
------------------|---------------------------|
testDlopen.c      | simple test for dlfcn     |
testDynOpen.c     | add more wrapper function |
testLazyDynOpen.c | add lazy-loading feature  |

## `dlopen` a library which depend on other library

`dlopen` will auto load the dependecny library as manual page.

However, we must indicate the dependency when link the first library.

we could use `ldd [libfirst.so]` to verify the dependency.
