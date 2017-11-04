#include <stdio.h>
#include <stdbool.h>

static int g = true;
bool reduceOne(char *in) {
    bool r = g;
    g = false;
    return r;
}

char *reduce(char *in) {
    // int cnt = 0;
    int cnt; // gcc cannot find this Wuninitialized error
    while (reduceOne(in)) {
        cnt++;
    }
    printf("expect 1, but get %d\n", cnt);
    return in;
}

int main() {
    reduce("");
    return 0;
}
