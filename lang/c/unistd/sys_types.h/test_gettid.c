#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#include <pthread.h>

#ifdef SYS_gettid
#define gettid() syscall(SYS_gettid)
#else
#error "SYS_gettid unavailable on this system"
#endif

int main() {
    printf("%ld\n", gettid());
    printf("%ld\n", pthread_self());
    // TODO: different result from gettid and pthread_self
    return 0;
}
