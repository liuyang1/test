#include <stdint.h>

struct {
    uint8_t a[42];
} magic_struct;

#define MAGICNUM_INTERNAL sizeof(magic_struct)
