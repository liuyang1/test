#include <stdio.h>
#include <limits.h>

// coverity[root_function]
int main(int argc, char **argv) {
    if (argc == 1) {
        printf("need one path as argv[1]\n");
        return 0;
    }
#if 1
    char *path = argv[1]; // WARN: path maniplution
#ifdef __COVERITY__
    __coverity_mark_pointee_as_sanitized__(&path, PATH);
#endif

#else
    char *path = "1.c"; // OK: fixed path name
#endif
    printf("path=%s\n", path);

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("fail to open path=%s\n", path);
        return 0;
    }
    printf("open path=%s succ\n", path);
    fclose(fp);
    return 0;
}
