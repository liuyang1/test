#include <stdio.h>

extern const char _binary_hello_start;
extern const char _binary_hello_end;
extern const int _binary_hello_size;

int main() {
    printf("start: %p\n", &_binary_hello_start);
    printf("end: %p\n", &_binary_hello_end);

    const char *p;
    for (p = &_binary_hello_start; p != &_binary_hello_end; p++) {
        printf("%c  ", *p);
    }
    printf("\n");
    for (p = &_binary_hello_start; p != &_binary_hello_end; p++) {
        printf("%02x ", *p);
    }
    printf("\n");

    int size = &_binary_hello_end - &_binary_hello_start;
    printf("size=%d\n", size);

    printf("%p\n", &_binary_hello_size);
    printf("%d\n", &_binary_hello_size);
    printf("%d\n", _binary_hello_size);
    size = &_binary_hello_size;
    printf("size=%d\n", size);

    return 0;
}
