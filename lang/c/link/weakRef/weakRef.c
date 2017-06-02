#include <stdio.h>
#include <pthread.h>

int __pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg)
{
    printf("call fake pthread_create func\n");
    return -1;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg)
    // __attribute__ ((weak, weakref, alias("__pthread_create")))
    __attribute__ ((weakref))
    ;

int main()
{
    // TODO: NOT work
    if (pthread_create) {
        printf("not find pthread_create\n");
    } else {
        printf("    find pthread_create=%p\n", pthread_create);
    }
    return 0;
}
