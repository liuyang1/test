#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    MODE_BAD,
    MODE_SERVER,
    MODE_CLIENT
};

void usage(char *cmd)
{
    printf("%s: test semaphore\n", cmd);
    printf("\t%s [server|client]\n", cmd);
}

int parse_arg(char *arg)
{
    if (strcmp(arg, "server") == 0) {
        return MODE_SERVER;
    } else if (strcmp(arg, "client") == 0) {
        return MODE_CLIENT;
    }
    return MODE_BAD;
}

const char sem_name[] = "/mytestsem";

int do_post(sem_t *sem)
{
    int ret, val;
    ret = sem_post(sem);
    printf("post sem.%s\n", sem_name);

    ret = sem_getvalue(sem, &val);
    printf("sem.%s=%d ret=%d\n", sem_name, val, ret);
    return ret;
}

int do_wait(sem_t *sem)
{
    int ret, val;
    ret = sem_getvalue(sem, &val);
    printf("sem.%s=%d ret=%d\n", sem_name, val, ret);

    printf("block wait sem.%s decrease to zero\n", sem_name);
    ret = sem_wait(sem);
    printf("get sem.%s ret=%d\n", sem_name, ret);

    ret = sem_getvalue(sem, &val);
    printf("sem.%s=%d ret=%d\n", sem_name, val, ret);

    return ret;
}
int main(int argc, char **argv)
{
    int mode;
    if (argc != 2 || ((mode = parse_arg(argv[1])) == MODE_BAD)) {
        usage(argv[0]);
        exit(-2);
    }

    int ret;
    // sem_unlink to remove semaphore. This function return immediately, but
    // semaphore will destory when all processes close it.
    // if semaphore already destoryed, it will return -1.
    if (mode == MODE_SERVER) {
        ret = sem_unlink(sem_name);
        printf("sem_unlink sem.%s ret=%d\n", sem_name, ret);
        if (ret != 0) {
            perror("sem_unlink");
        }
    }

    // sem_open create a new semaphore or open an existing semaphore.
    // The semaphore is indentified by NAME.
    // open may fail if just previous same name semaphore not destory.
    // it will create a virtual file "sem." + sem_name in `/dev/shm`.
    sem_t *sem = sem_open(sem_name, O_CREAT, 0777, 0);
    if (sem == SEM_FAILED) {
        perror("open sem");
        exit(-1);
    }
    printf("create sem.%s=%p\n", sem_name, sem);

    switch (mode) {
        case MODE_SERVER:
            printf("{{{ wait self process's semaphore\n");
            do_post(sem);
            do_wait(sem);
            printf("}}} wait self process's semaphore, DONE\n");
            printf("{{{ wait client process's semaphore\n");
            do_wait(sem);
            printf("}}} wait client process's semaphore, DONE\n");
            break;
        case MODE_CLIENT:
            printf(">>> post to server process\n");
            do_post(sem);
            break;
        default:
            printf("should not here\n");
            break;
    }

    ret = sem_close(sem);
    printf("close sem.%s ret=%d\n", sem_name, ret);
    // ~~~~ ret = sem_unlink(sem_name);
    // after close, better not unlink it, else next create may fail with
    // "Invalid arguments"
    return 0;
}
