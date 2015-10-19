#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void TimeoutHandler(int arg)
{
    if (arg == SIGALRM) {
        printf("timeout arg=%d\n", arg);
    } else {
        printf("arg=%d other signal\n", arg);
    }
}

int testFunc()
{
    printf("enter %s\n", __FUNCTION__);
    {
        sleep(1);
    }
    printf("leave %s\n", __FUNCTION__);
    return 0;
}

int main()
{
    signal(SIGALRM, TimeoutHandler);
    int dt;

    dt = alarm(3);
    if (dt != 0) {
        printf("cancel previous alram, still have dt=%d to end\n", dt);
    }

    {
        testFunc();
    }

    dt = alarm(0);
    if (dt == 0) {
        printf("alarm timeout\n");
    } else {
        printf("cancel alarm, sitll have dt=%d to end\n", dt);
    }
    return 0;
}
