/* The program creates a child process and waits for it to finish. If a timeout
 * elapses the child is killed. Waiting is done using sigtimedwait(). Race
 * condition is avoided by blocking the SIGCHLD signal before fork().
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void childFunc()
{
    int dt = 3;
    printf("child: sleeping %d seconds...\n", dt);
    sleep(dt);
    puts("child: exiting");
}

static pid_t fork_child(void)
{
    int p = fork();
    if (p == -1) {
        perror("fork error");
        exit(1);
    }
    if (p != 0) {
        printf("return to parent\n");
        return p;
    }
    printf("child process\n");
    childFunc();
    exit(0);
}

int main()
{
    sigset_t mask;
    sigset_t orig_mask;
    struct timespec timeout;
    pid_t pid;

    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);

    if (sigprocmask(SIG_BLOCK, &mask, &orig_mask) < 0) {
        perror("sigprocmask");
        return 1;
    }

    pid = fork_child();

    timeout.tv_sec = 5;
    timeout.tv_nsec = 0;

    printf("start of main thread\n");
    do {
        if (sigtimedwait(&mask, NULL, &timeout) < 0) {
            if (errno == EINTR) {
                /* Interrupted by a signal other than SIGCHLD. */
                continue;
            } else if (errno == EAGAIN)   {
                printf("Timeout, killing child\n");
                kill(pid, SIGKILL);
            } else {
                perror("sigtimedwait");
                return 1;
            }
        }

        break;
    } while (1);

    if (waitpid(pid, NULL, 0) < 0) {
        perror("waitpid");
        return 1;
    }
    printf("end of main thread\n");

    return 0;
}
