/** demo for async-await in C
 * - RPC to thread
 * - async call to return a promise
 * - await to block to get result from promise
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>

typedef void *(*Job)(void *);

typedef struct {
    int id;
    Job job;
    void *arg;
    void *ret;
} MsgFmt;

typedef struct {
    mqd_t slave;
    mqd_t master;
} Portal;

void *worker(void *arg) {
    Portal *p = arg;
    mqd_t rx = p->slave;
    mqd_t tx = p->master;
    MsgFmt msg, *m = &msg;
    while (true) {
        int len = mq_receive(rx, (void *)m, sizeof(MsgFmt), NULL);
        // printf("len=%d %lu err=%d/%s\n", len, sizeof(MsgFmt), errno,
        //        strerror(errno));
        assert(len == sizeof(MsgFmt));

        if (m->job == NULL) { // terminate
            printf("finish at id=%d\n", m->id);
            break;
        }
        m->ret = m->job(m->arg);

        mq_send(tx, (void *)m, len, 0);
    }
    return NULL;
}

typedef struct {
    int exn;
    int op0, op1;
} MathArg;
void *add(void *arg) {
    MathArg *p = arg;
    p->op0 = p->op0 + p->op1;
    return p;
}
void *minus(void *arg) {
    MathArg *p = arg;
    p->op0 = p->op0 - p->op1;
    return p;
}
void *mul(void *arg) {
    MathArg *p = arg;
    p->op0 = p->op0 * p->op1;
    return p;
}
void *mydiv(void *arg) {
    MathArg *p = arg;
    if (p->op1 == 0) {
        p->exn = -1;
        return p;
    }
    p->op0 = p->op0 / p->op1;
    return p;
}

static id = 0;

void *RPC(Portal *p, Job job, void *arg) {
    MsgFmt m = {
        .id = id++,
        .job = job,
        .arg = arg,
    };
    mq_send(p->slave, (void *)&m, sizeof(m), 0);
    mq_receive(p->master, (void *)&m, sizeof(m), NULL);
    return m.ret;
}

int async(Portal *p, Job job, void *arg) {
    MsgFmt m = {
        .id = id++,
        .job = job,
        .arg = arg,
    };
    mq_send(p->slave, (void *)&m, sizeof(m), 0);
    return m.id;
}
void *await(Portal *p, int id) {
    MsgFmt m;
    mq_receive(p->master, (void *)&m, sizeof(m), NULL);
    return m.ret;
}

void terminate(Portal *p) {
    MsgFmt m = {
        .id = id++,
        .job = NULL,
    };
    mq_send(p->slave, (void *)&m, sizeof(m), 0);
}

Portal port_create() {
    /** mq usage
     * 0, create it not exist
     * 1, name with "/xxxx"
     */
    mq_unlink("/master"); // remove existing mq
    mq_unlink("/slave");
    Portal port;
    int msgn = 10;
    int flag = O_RDWR | O_CREAT;
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = msgn,
        .mq_msgsize = sizeof(MsgFmt),
        .mq_curmsgs = 0,
    };
    port.master = mq_open("/master", flag, 0666, &attr);
    port.slave = mq_open("/slave", flag, 0666, &attr);
    printf("port flag=%#x %d %d msg=%dx%ld\n", flag, port.master, port.slave,
           msgn, sizeof(MsgFmt));
    if (port.master == -1 || port.slave == -1) {
        printf("err %d/%s\n", errno, strerror(errno));
        exit(-1);
    }

    struct mq_attr cfg;
    mq_getattr(port.master, &cfg);
    printf("mq_attr %lx %ldx%ld %ld\n", cfg.mq_flags, cfg.mq_maxmsg, cfg.mq_msgsize,
           cfg.mq_curmsgs);

    return port;
}

void port_destroy(Portal *port) {
    mq_close(port->master);
    mq_close(port->slave);
}

int main() {
    Portal port = port_create();

    pthread_t task;
    pthread_create(&task, NULL, worker, &port);

    MathArg arg = {.op0 = 2, .op1 = 40};
    MathArg *r = RPC(&port, add, &arg);
    printf("r=%d %d\n", r->op0, r->op1);

    int promise = async(&port, add, &arg);
    r = await(&port, promise);
    printf("r=%d %d\n", r->op0, r->op1);

    terminate(&port);
    pthread_join(task, NULL);

    port_destroy(&port);
    return 0;
}
