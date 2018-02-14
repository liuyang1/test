#include <stdio.h>
#include <features.h>
#define PRINTVAL(x) printf("%s: %ld %#lx\n", #x, x, x);

int main() {
    PRINTVAL(_POSIX_C_SOURCE);
    return 0;
}
