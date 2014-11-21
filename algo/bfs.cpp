 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(x,y)    (x<y?x:y)
#define BULKNUM     3
#define SETSPACE    1024
#define COMNUM      (BULKNUM*(BULKNUM-1))

/*
 * 这是一个灌水问题.
 * 已知有3个桶,容积为10,7,4.初始水量为10,0,0
 * 求如何得到2份水的分量
 *
 * 这里采用了状态空间搜索的方法
 */
// int VOLUME[BULKNUM] = { 10, 7, 4};
// int INITST[BULKNUM] = { 10, 0, 0};
int VOLUME[BULKNUM] = { 7, 5, 3};
int INITST[BULKNUM] = { 0, 5, 3};

int trans(int* st,int from,int to){
    int res = st[from];
    int req = VOLUME[to] - st[to];
    if(req<=0)
        return false;
    int val = min(res,req);
    st[from] -= val;
    st[to]   += val;
    return true;
}

int FROMTO[COMNUM][2]= { {0,1},{1,0},{1,2},{2,1},{0,2},{2,0}};

bool isEqual(int *st,int* ST){
    for(int i=0;i<BULKNUM;i++){
        if(st[i]!=ST[i])
            return false;
    }
    return true;
}
bool isRep(int st[BULKNUM],int STset[][BULKNUM],int n)
{
    for(int i=0;i<n;i++){
        if(isEqual(st,STset[i]))
            return true;
    }
    return false;
}

int chkisEqual(){
    int a[3] = {1,2,3};
    int b[3] = {1,2,3};
    if(isEqual(a,b)==false){
        printf("wrong isEqual\n");
    }
    int c[3] = {1,3,3};
    if(isEqual(a,c)==true)
        printf("wrong isEqual\n");
    int d[2][3]={{1,3,3},{1,2,3}};
    if(isRep(a,d,2)==false)
        printf("wrong isEqual\n");
    int e[2][3]={{1,3,3},{1,3,2}};
    if(isRep(a,e,2)==true)
        printf("wrong isEqual\n");
    printf("logic right\n");
    return 0;
}
int search(){
    int STset[SETSPACE][BULKNUM];
    memcpy(STset[0],INITST,BULKNUM);

    int setp=0;
    int setn=1;

    bool ret;
    int sttmp[BULKNUM];
    while(1){
        for(int i=0;i<COMNUM;i++){
            memcpy(sttmp,STset[setp],BULKNUM*sizeof(int));
            ret = trans(sttmp,FROMTO[i][0],FROMTO[i][1]);
            if(ret==true){
                if(isRep(sttmp,STset,setn))
                    continue;
                printf("%d %d %d",STset[setp][0],STset[setp][1],STset[setp][2]);
                printf("\t%d->%d\t",FROMTO[i][0],FROMTO[i][1]);
                printf("%d %d %d\n",sttmp[0],sttmp[1],sttmp[2]);
                memcpy(STset[setn],sttmp,BULKNUM*sizeof(int));
                setn++;
                if(setn>=SETSPACE){
                    printf("no enough space\n");
                    return -1;
                }
            }
        }
        setp++;
        if(setp==setn)
            break;
    }
    return 0;
}

int main(){
    // chkisEqual();
    search();
    return 0;
}
