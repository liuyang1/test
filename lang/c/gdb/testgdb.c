#include <unistd.h>
#include <stdio.h>

int main()
{
    int flag = 1;
    int val = 0;
    while (flag) {
        sleep(1);
        printf("val=%d\n", val);
        val++;
        if (val == 20)
            break;
        printf("next loop\n");
    }
    return 0;
}
