#include <stdio.h>
#include "tbl.h"

typedef int ErrorCode;

int loopOnData(char* in, int len, int (*func)(char))
{
    int s = 0;
    for(int i=0; i < len; i++, in++)
        s += func(*in);
    return s;
}

int NoZero(char c)
{
    return c != 0;
}

int totalCoeffs(char* in, int len)
{
    return loopOnData(in, len, NoZero);
}

int trailingsOnes(char *in, int len)
{
    int s = 0;
    in+=len-1;
    for(int i=0; i < len; i++, in--){
        if(*in == 0)
            s++;
        if(s == 3 || *in != 0)
            return s;
    }
    return s;
}
int totalZeros(char *in, int len)
{
    int s = 0;
    for(int i=0; i < len; i++, in++){
        if(*in == 0)
            s++;
    }
    in--;
    for(int i=0; i < len; i++, in--){
        if(*in == 0)
            s--;
        else
            break;
    }
    return s;
}
void writeData(char* data, char c, int n)
{
    for(int i=0; i < n; i++, data++)
        *data = c;
}
        
ErrorCode cavlc(char* in, int insize, char* out, int outsize)
{
    int totalc=0, trail=0, totalz=0;
    totalc = totalCoeffs(in, insize);
    trail = trailingsOnes(in, insize);
    totalz = totalZeros(in, insize);
    printf("%d %d %d\n", totalc, trail, totalz);
    char buf[256];
    int bitoffset = writeCoeffToken(buf, 0, trail, totalc);
    for(int i=0; i<bitoffset/8+1; i++)
        printf("0x%02x\n", buf[i]);
    printf("bitoffset %d\n", bitoffset);
    return 0;
}

void test()
{
    char a[] = {0, 3, 0, 1, -1,
        -1, 0, 1, 0, 0,
        0, 0, 0, 0, 0,
        0};
    int len = sizeof(a) / sizeof(a[0]);
    char buf[256];
    cavlc(a, len, buf, 256);
}

int main()
{
    test();
    return 0;
}
