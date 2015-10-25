#define _GNU_SOURCE
// support pthread_timedjoin_np function
// This macro work in <features.h>
// define it before all included header file
#include <stdio.h>
#include <errno.h>

#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define NSEC_PER_SEC_U64 (1*1000ull*1000*1000)

struct timespec afterNanoSec(unsigned long long nanosec)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    nanosec += now.tv_nsec;
    now.tv_sec += nanosec / NSEC_PER_SEC_U64;
    now.tv_nsec = nanosec % NSEC_PER_SEC_U64;
    return now;
}

struct timespec afterSec(unsigned long long sec)
{
    return afterNanoSec(NSEC_PER_SEC_U64 * sec);
}

struct timespec now()
{
    return afterNanoSec(0);
}

void *func(void *pdt)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    int dt = *(int *)pdt;
    printf("main: long time function, wait dt=%u\n", dt);

    sleep(dt);

    printf("main: long time function end\n");

    return NULL;
}

void timedCallFuncWithJoin(void *(*func)(void *), void *pArgs, int t)
{
    pthread_t thread1;

    pthread_create(&thread1, NULL, func, pArgs);

    void *retval;
    struct timespec ts = afterSec(t);
    int ret = pthread_timedjoin_np(thread1, &retval, &ts);

    if (ret == ETIMEDOUT) {
        printf("thread cannot join as timeout, cancel it\n");
        ret = pthread_cancel(thread1);
        if (ret != 0) {
            printf("pthread_cancel ret=%x\n", ret);
        }
        ret = pthread_join(thread1, &retval);
    }
    if (retval == PTHREAD_CANCELED) {
        printf("thread join with cancel\n");
    } else {
        printf("thread join with retval=%p ret=%x\n", retval, ret);
    }
}

void *g_handlerArg;

void timeoutHandler()
{
    if (g_handlerArg == NULL) {
        printf("arg == NULL\n");
        return;
    }
    pthread_t *p = g_handlerArg;
    printf("timeout pthread cancel\n");
    int ret = pthread_cancel(*p);
    if (ret != 0) {
        printf("pthread_cancel ret=%x\n", ret);
    }
}

void signalHandler(int arg)
{
    printf("signal %x\n", arg);
    switch (arg) {
        case SIGALRM:
            timeoutHandler();
            break;
        default:
            break;
    }
}

void timedCallFuncWithAlarm(void *(*func)(void *), void *pArgs, int t)
{
    signal(SIGALRM, signalHandler);
    pthread_t thread1;
    g_handlerArg = &thread1;
    int dt = alarm(t);
    if (dt != 0) {
        printf("cancel previous alarm, still have dt=%d to end\n", dt);
    }

    pthread_create(&thread1, NULL, func, pArgs);
    void * retval;
    int ret = pthread_join(thread1, &retval);
    if (retval == PTHREAD_CANCELED) {
        printf("thread join with cancel\n");
    } else {
        printf("thread join with retval=%p ret=%x\n", retval, ret);
    }

    g_handlerArg = NULL;
    dt = alarm(0);
    if (dt == 0) {
        printf("alarm timeout\n");
    } else {
        printf("cancel alarm, still have dt=%d to end\n", dt);
    }
}

int main() {
    int dt;
    while (1) {
        printf("\n---- test case running ----\n");
        printf("timed with 2 sec, input function eslaped time(>60, exit):");
        scanf("%d", &dt);
        if (dt > 60) {
            break;
        }
        struct timespec ts0 = now();
        printf("current time: %u sec %llu ns\n", ts0.tv_sec, ts0.tv_nsec);

        // which type of this function pointer? :)
        void (*timedCallFunc)(void *(*func)(void *), void *pArgs, int t);
        timedCallFunc = timedCallFuncWithJoin;
        // timedCallFunc = timedCallFuncWithAlarm;
        timedCallFunc(func, &dt, 2);

        struct timespec ts1 = now();
        printf("current time: %u sec %llu ns\n", ts1.tv_sec, ts1.tv_nsec);

        printf("should use time min(dt, 2)=%d\n", dt < 2 ? dt : 2);
        unsigned long long sec, nsec;
        if (ts1.tv_nsec > ts0.tv_nsec) {
            nsec = ts1.tv_nsec - ts0.tv_nsec;
            sec = ts1.tv_sec - ts0.tv_sec;
        } else {
            nsec = NSEC_PER_SEC_U64 + ts1.tv_nsec - ts0.tv_nsec;
            sec = ts1.tv_sec - ts0.tv_sec - 1;
        }

        printf("eslaped time: %10u sec %9llu ns\n", sec, nsec);
    }

    return 0;
}
