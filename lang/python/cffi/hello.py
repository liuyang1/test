import cffi

ffi = cffi.FFI()
ffi.cdef("""
    int hello(char *);
""")
# This r(RAW) forbidden newline char
lib = ffi.verify(r"""
#include <stdio.h>
    int hello(char *name) {
        printf("hello, %s\n", name);
        return 0;
    }
""")

lib.hello(b"world")
