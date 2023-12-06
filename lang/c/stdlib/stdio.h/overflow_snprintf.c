#include <string.h>
#include <stdio.h>

int main() {
    char *a[] = {"test", "hello", "world", "yangyang"};
#define N 16
    char s[N];
    size_t i;
    size_t c = 0;
    int ret;
    for (i = 0; i != sizeof(a)/ sizeof(a[0]); i++) {
        if (c + strlen(a[i]) >= N) {
            break;
        }
        c += ret = snprintf(s + c, N - c, "%s", a[i]);
        printf("ret=%d c=%zu\n", ret, c);
    }
    return 0;
}
