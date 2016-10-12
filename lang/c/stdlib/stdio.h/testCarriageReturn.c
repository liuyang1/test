#include <stdio.h>
#include <unistd.h>

int main() {
    int i;
    for (i = 0; i < 10; i++) {
        printf("\rfinished %3d\r", i);
        // if not have this line, get nothing output when interacive mode
        fflush(stdout);
        sleep(1);
    }
    return 0;
}
