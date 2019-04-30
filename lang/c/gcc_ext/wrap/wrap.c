#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifdef WRAP
int __real_printf(const char *format, ...);
int __wrap_printf(const char *format, ...) {
    __real_printf("calling into my printf with format=%s\n", format);
    va_list args;
    va_start(args, format);
    int r = vprintf(format, args);
    va_end(args);
    return r;
}

void *__real_malloc(size_t size);
void *__wrap_malloc(size_t size) {
    __real_printf("calling into my malloc with sz=%d\n", size);
    void *p = (void *)__real_malloc(size);
    return p;
}

#endif

int main() {
#ifdef WRAP
    __real_printf("enter main\n");
#else
    printf("enter main\n");
#endif
    const char s[] = "Hello, World\n";
    printf("%s\n", s); // it optimize to `puts`, defaulut
    printf("%d\n", 42);

    char *p = malloc(256);
    memcpy(p, s, sizeof(s));
    printf("s=%s p=%s\n", s, p);
    free(p);
    return 0;
}
