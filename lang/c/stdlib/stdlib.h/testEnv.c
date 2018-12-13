#include <stdio.h>
#include <stdlib.h>

void showEnv(const char *name) {
    char *env = getenv(name);
    printf("env[%s] = %s\n", name, env);
}

int main() {
    const char *name = "LD_LIBRARY_PATH";
    showEnv(name);

    int ret = setenv(name, "./:../", 1);

    showEnv(name);
    return 0;
}
