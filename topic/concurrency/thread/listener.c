#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAXNUM      6

typedef int (*EvCallback)(int id, void *msg);
enum {
    eUndefinedId,
};
typedef struct {
    int id;
    EvCallback onEvent;
} Listener;

static Listener NullLis = {.id = eUndefinedId, .onEvent = NULL};
static Listener gListeners[MAXNUM] = {{eUndefinedId, NULL}};
static pthread_mutex_t gLisMutex;

#define assertId(id) {if (id == eUndefinedId) { return false; }}
#define assertFn(pt) {if (pt == NULL) { return false; }}

void initListenerGroup() {
    pthread_mutex_init(&gLisMutex, NULL);
}

void finiListenerGroup() {
    pthread_mutex_destroy(&gLisMutex);
}

int repListener(Listener lis0, Listener lis1) {
    int ret = false;
    int i;
    pthread_mutex_lock(&gLisMutex);
    for (i = 0; i != MAXNUM; i++) {
        if (gListeners[i].id == lis0.id) {
            gListeners[i] = lis1;
            ret = true;
            goto final;
        }
    }
final:
    pthread_mutex_unlock(&gLisMutex);
    return ret;
}

/*
 * Notify process need call ALIEN method.
 * So it may acquire any lock, or attch / detach listener.
 * DONOT lock on ALIEN method.
 * 1. avoid calling alien method when holding a lock
 * 2. minimize period during holding lock
 * 3. alien methold could do anything, including attach or detach listener
 */
void notifyListener(void *msg) {
    int i;
    for (i = 0; i != MAXNUM; i++) {
        if (gListeners[i].id != eUndefinedId) {
            // gListeners[i].onEvent(gListeners[i].id, msg);
            pthread_mutex_lock(&gLisMutex);
            Listener lis = gListeners[i];
            pthread_mutex_unlock(&gLisMutex);

            lis.onEvent(lis.id, msg);
        }
    }
}

/* API */
int attachListener(Listener lis) {
    assertId(lis.id);
    assertFn(lis.onEvent);
    return repListener(NullLis, lis);
}

int detachListener(Listener lis) {
    assertId(lis.id);
    // when detach, not need to check Event function
    return repListener(lis, NullLis);
}

// test code
int SilentListener(int id, void *msg) {
    printf("silent  lis=%04d recv msg=%p\n", id, msg);
    return 0;
}

int OneTimeListener(int id, void *msg) {
    printf("onetime lis=%04d recv msg=%p\n", id, msg);
    Listener self = {.id = id, NULL};
    int r = detachListener(self);
    if (!r) {
        printf("cannot remove listener id=%d\n", id);
    }
    return 0;
}

int ChaosListener(int id, void *msg) {
    printf("chaos   lis=%04d recv msg=%p\t", id, msg);
    if (msg != NULL) {
        char *str = msg;
        printf("msg=[%s]\n", str);
    }
    Listener self = {.id = id, NULL};
    Listener lis0 = {.id = id * 2, ChaosListener};
    Listener lis1 = {.id = id * 2 + 1, OneTimeListener};
    detachListener(self);
    attachListener(lis0);
    attachListener(lis1);
    return 0;
}

void *notifyTask(void *args) {
    char msg[16];
    int i;
    for (i = 0; i != 5; i++) {
        sleep(1);
        sprintf(msg, "msg=%d", i);
        notifyListener(msg);
    }
    return NULL;
}

int main() {
    initListenerGroup();
    pthread_t t0;
    pthread_create(&t0, NULL, notifyTask, NULL);

    Listener silent = {1, SilentListener};
    Listener chaos = {2, ChaosListener};
    Listener one = {3, OneTimeListener};
    attachListener(silent);
    attachListener(chaos);
    attachListener(one);

    pthread_join(t0, NULL);
    finiListenerGroup();
    return 0;
}
