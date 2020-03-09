#include <stdio.h>

#if 1
#define INCBIN(name, file)                 \
    __asm__ (                              \
        ".section .rodata\n"               \
        ".global "# name "\n"              \
        ".type "# name ", @object\n"       \
        ".align 16\n"                      \
        # name ": .incbin  \"" file "\"\n" \
                                           \
        ".global "# name "_len\n"          \
        ".type "# name "_len, @object\n"   \
        ".align 16\n"                      \
        # name "_len: .int "# name "_len - "# name "\n");

INCBIN(my_data, "song.mp3")
#else



__asm__ (                             \
    ".section .rodata\n"              \
    ".global my_data\n"               \
    ".type my_data, @object\n"        \
    ".align 16\n"                     \
    "my_data: .incbin \"song.mp3\"\n" \
                                      \
    ".global my_data_len\n"           \
    ".type my_data_len, @object\n"    \
    ".align 16\n"                     \
    "my_data_len: .int my_data_len - my_data\n");
#endif
