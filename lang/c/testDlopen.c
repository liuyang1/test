#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#define Log         printf
int main(int argc, char **argv) {
    void *handle;
    double (*cosine)(double);
    char *error;

    // handle = dlopen ("/lib/i386-linux-gnu/libm.so.6", RTLD_LAZY);
    char *dlfn = "/lib/x86_64-linux-gnu/libm.so.6";
    Log("loading dl=%s\n", dlfn);
    handle = dlopen (dlfn, RTLD_LAZY);
    if (!handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }

    char *symstr = "cos";
    Log("loading symbol=%s\n", symstr);
    cosine = dlsym(handle, symstr);
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        exit(1);
    }

    Log("run symbol\n");
    printf ("%f\n", (*cosine)(2.0));
    dlclose(handle);
}
