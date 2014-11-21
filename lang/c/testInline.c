#include <stdio.h>

inline int square(int x)
{
    return x * x;
}

int main()
{
    printf("%d\n", square(2));
    return 0;
}
