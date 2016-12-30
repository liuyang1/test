#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#define Log         printf

double (*cosine)(double) = NULL;
void *handle = NULL;

int tryLoad() {
    char *error;

    // handle = dlopen ("/lib/i386-linux-gnu/libm.so.6", RTLD_LAZY);
    const char *dlfn = "/lib/x86_64-linux-gnu/libm.so.6";
    // const char *dlfn = NULL;
    Log("loading dl=%s\n", dlfn);
    handle = dlopen (dlfn, RTLD_LAZY);
    if (!handle) {
        fputs (dlerror(), stderr);
        exit(1);
    } else {
        Log("load handle=%p\n", handle);
    }

    const char *symstr = "cos";
    Log("loading symbol=%s\n", symstr);
    cosine = dlsym(handle, symstr);
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        fputs("\n", stderr);
        // if load fail, still continue
        // exit(1);
    } else {
        Log("load cos=%p\n", cosine);
    }
}

int tryUnload() {
    if (handle) {
        dlclose(handle);
    }
}

double consine_wrap(double x) {
    return cosine(x);
}

int main()
{
    tryLoad();

    Log("run symbol\n");
    printf("%f\n", consine_wrap(2.0));

    tryUnload();
    return 0;
}
