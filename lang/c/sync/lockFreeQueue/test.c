#include <stdint.h>
#include <stdio.h>
#include "queue.h"

const char *showBool(bool b) {
    return b ? "true" : "false";
}

#define EXPECT(v, e) printf("%d ?= %d %s\n", v, e, showBool(v == e));
#define VERBOSE

#ifdef VERBOSE
#define CHECK(b, msg) printf("%s %s\n", msg, b ? "success" : "fail");
#else
#define CHECK(b, msg) if (!b) {printf("%s fail\n", msg);}
#endif

bool test_simple() {
#define N 100
    bool ret = true;
    queue *q = open();
    for (long i = 0; i != N; i++) {
        enqueue(q, (void *)i);
    }
    for (long i = 0; i != N; i++) {
        void *p = dequeue(q);
        if (p != (void *)i) {
            ret = false;
        }
    }
    close(q);
    return ret;
}
int main() {
    bool ret = test_simple();
    CHECK(ret, "test_simple case");

    return 0;
}
