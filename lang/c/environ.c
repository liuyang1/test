#include <stdio.h>
extern char **environ;

int main()
{
    char **env;
    for (env = environ; *env; ++env) {
        printf("%s\n", *env);
    }
    return 0;
}
