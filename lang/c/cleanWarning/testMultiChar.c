#include <stdio.h>

#define FCC_GEN(a, b, c, d)     (((d)<<24) |((c)<<16) | ((b)<<8) | (a))

int main() {
    printf("%x\n", 'G');
    printf("%x\n", 'DP');
    printf("%x\n", 'PD');
    printf("%x\n", FCC_GEN('P', 'D', '\0', '\0'));
    // 'DP' is equal to FCC_GEN('P', 'D', '\0', '\0')
    // reverse order
    // this related to endianess issue.
    return 0;
}
