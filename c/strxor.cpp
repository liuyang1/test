#include <stdio.h>

void xorstr(char* a, char* b,int n)
{
    for(int i = 0; i < n || *b != '\0'; i++, a++, b++)
        *a = *a ^ *b;
}

int main(){
    char a[10]={0};
    char b[4]="abc";
    xorstr(a,b,10);
    printf("%s\n",a);

    char c[10] = "de";
    xorstr(a,c,10);
    printf("%s\n",a);

    xorstr(a,b,10);
    printf("%s\n",a);

    printf("%d\n",10^0);
    return 0;
}
