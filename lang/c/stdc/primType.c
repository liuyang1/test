#include <stdio.h>

#define SHOW_TYPE(x)    printf("sizeof(%s)=%lu\n", #x, sizeof(x));
int main() {
    SHOW_TYPE(char);
    SHOW_TYPE(short);
    SHOW_TYPE(long);
    SHOW_TYPE(long long);
    SHOW_TYPE(void *);
    SHOW_TYPE(float);
    SHOW_TYPE(double);
    return 0;
}
