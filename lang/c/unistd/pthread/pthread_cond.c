/**
 * @file pthread_cond.c
 * @brief demo program for pthread_cond_t
 * @ref  https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032r/index.html
 * @author liuyang<ly@marvell.com>
 * @version 0.0.1
 * @date 2016-12-03
 */
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t gMutex;
pthread_cond_t gCond;

unsigned int gCount;

void *cond_loop(void *arg) {
    const char *name = arg;

    while (true) {
        pthread_mutex_lock(&gMutex);
        /**  ** fake wake-up issue **
         * Because the condition can change before an awakened thread returns from
         * pthread_cond_wait(), the condition that caused the wait must be retested before the mutex
         * lock is acquired.
         */
        while (gCount == 0) {
            printf("%s wait\n", name);
            /** This function blocks until the condition is signaled. It atomically releases the
             * associated mutex lock before blocking, and atomically acquires it again before
             * returning.
             */
            /** > cond must work with mutex, as it unlock mutex internal;
             *  > so outside, could lock it again before signal or boardcast;
             *  > after it wait the signal, it will lock again,
             *  > so it could unlock after handler.*/
            pthread_cond_wait(&gCond, &gMutex);
        }
        pthread_mutex_unlock(&gMutex);

        sleep(3);
        gCount--;
        printf("%s decrease\n", name);
        /** ** The Lost Wake-Up Problem **
         *  When no threads are lbocked on the condition variable,
         *  calling `pthread_cond_signal` have no effect;
         *
         *  > so if we have `sleep` at here, and quick send multiple signal from main thread,
         *  > only first one have effect;
         *
         *  >> This point is quite different with seamphere;
         *  >> With seamphere, it always have effect to up value;
         *
         *  >>> This problem may enlarge if signal pthread must wait gCount reset;
         *  >>> As the signal is lost, so wait thread will keep wait status forever;
         *  >>> If signal thread wait gCount reset, then we have dead-lock issue.
         */


    }
    printf("should not be here\n");
    return NULL;

}

int main() {
#define LEN 1024
    char s[LEN];
    char *p;
    gCount = 0;
    pthread_mutex_init(&gMutex, NULL);
    pthread_cond_init(&gCond, NULL);

    pthread_t t0, t1;
    pthread_create(&t0, NULL, cond_loop, "A");
    // pthread_create(&t1, NULL, cond_loop, "B");

    while (true) {
        printf("wait for input (q to quit): ");
        p = fgets(s, LEN, stdin);
        char c = p[0];
        if (c == 'q') {
            break;
        }

        pthread_mutex_lock(&gMutex);
        if (gCount == 0) {
            /** ** multiple context switch defect **
             *
             *                      wake-up                     return cond_wait
             *                      v   mutex-lock in wait      v
             * T2                   ----                         ----------------
             *  block on cond var   |  | block on mutex          |
             *                      |  |                         |
             * T1 -------------------  ---------------------------
             *                      ^                           ^
             *                      signal                      mutex-unlock
             *
             * So after signal, we have three times context switch
             * - T1 signal
             * - T2 wake up, then lock again in pthread_cond_wait function
             * - T1 switch back, T1 unlock
             * - T2 continue to run, T2 pthread_cond_wait return
             *
             * > wati morphing optimize
             * > when lock, directly move thread from queue of cond_wait to queue of mutex, and
             * > extra context switch
             */
            pthread_cond_signal(&gCond);
        }
        gCount++;
        pthread_mutex_unlock(&gMutex);

    }

    pthread_cond_destroy(&gCond);
    pthread_mutex_destroy(&gMutex);

    return 0;
}
