#include <stdio.h>
int main()
{
    printf("%s", NULL);
    void *ptr = NULL;
    printf("%s", ptr);
    printf("%d", (int)(*NULL));
    return 0;
}
