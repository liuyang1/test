#include <stdlib.h>
#include <stdio.h>
extern char **environ;

int main()
{
    char **env;
    for (env = environ; *env; ++env) {
        printf("%s\n", *env);
    }
    const char *key = "USER";
    char *path = getenv(key);
    printf("getenv to get %s: %s\n", key, path);
    return 0;
}
