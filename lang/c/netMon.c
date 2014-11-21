/*
 * ========================================================
 *
 *       Filename:  netMon.c
 *
 *    Description:  网络监控的程序,读取系统文件/proc/net/dev进行分析得到的数据
 *
 *        Version:  1.0
 *        Created:  08/09/2013 01:09:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */

#include <stdio.h>
#include <string.h>

static char devFile[] = "/proc/net/dev";

enum MonType
{
    NONE=0,
    RXbytes,
    RXpkts,
    TXbytes,
    TXpkts,
    ALLbytes,
    ALLpkts,
};

void skipline(FILE* fp)
{
    int ch;
    do{
        ch=getc(fp);
    }while(ch !='\n' && ch !=EOF);
}
int strSpaceEqu(char* s1,char* s2){
    while(1){
        while(*s1==' ')
            s1++;
        while(*s2==' ')
            s2++;
        if(*s1==*s2){
            if(*s1=='\0')
                return 1;
            s1++;
            s2++;
        }else
            return 0;
    }
}
unsigned int monitorDev(char* ifname,enum MonType type)
{
    FILE* fp = fopen(devFile,"r");
    if(fp == NULL){
        return 0;
    }
    
    skipline(fp);
    skipline(fp);
    
    char name[8];
    unsigned int rxbytes=0,rxpkts=0,txbytes=0,txpkts=0;

    do{
        unsigned int v1,v2,v3,v4;
        int val = fscanf(fp,"%6[^:]:%u %u %*u %*u %*u %*u %*u %*u"
                " %u %u",
                name,&v1,&v2,&v3,&v4);
        if(val==EOF)
            break;
        rxbytes += v1;
        rxpkts += v2;
        txbytes += v3;
        txpkts += v4;
        printf("%s : %u %u %u %u\n",name,rxbytes,rxpkts,txbytes,txpkts);
        skipline(fp);
    }while( !strSpaceEqu(name,ifname) || strcmp("all",ifname)==0 );

    printf("%s : %u %u %u %u\n",name,rxbytes,rxpkts,txbytes,txpkts);

    unsigned int ret=0;
    switch(type){
        case NONE:     ret=0;break;
        case RXbytes:  ret=rxbytes;break;
        case RXpkts:   ret=rxpkts;break;
        case TXbytes:  ret=txbytes;break;
        case TXpkts:   ret=txpkts;break;
        case ALLbytes: ret=rxbytes+txbytes;break;
        case ALLpkts:  ret=rxpkts+txpkts;break;
    }

    fclose(fp);
    return ret;
}

int main()
{
    unsigned int val = monitorDev("all",ALLbytes);
    printf("%u\n",val);
    return 0;
}
