#include <stdio.h>

int main(void)
{
    int *a = (int *)0xdeadbe00;
    int *b = a + 1;
    printf("a = %p; b = %p; b - a = %lx\n", a, b, b - a);

    double *pda = (double *)0xf000be01;
    double *pdb = pda + (b - a);
    printf("pda = %p; pdb = %p; pdb - pda = %lx\n", pda, pdb, pdb - pda);

    a = (int *)0xdeadbe00;
    b = (int *)0xdeadbe09;
    printf("a = %p; b = %p; b - a = %lx\n", a, b, b - a);
    printf("WARN: when address difference between pointer not\n"
           "\twill remainder\n");
    return 0;
}
