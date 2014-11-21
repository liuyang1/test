/*
 * =============================================================
 *
 *       Filename:  testClassVar.cpp
 *
 *    Description:  检测成员变量的继承的情况
 *
 *        Version:  1.0
 *        Created:  11/15/2012 03:54:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * =============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
class Father
{
    private:
        char name[1024];
    protected:
    public:
        int varPri;
        int varPro;
        int varPub;
        void toStr(char *str,int len){
            snprintf(str,len,"name : %s \n\tpri : %d,pro : %d,pub : %d\n",name,varPri,varPro,varPub);
        }
        Father(int _v1,int _v2,int _v3,const char * _n):varPri(_v1),varPro(_v2),varPub(_v3)
        {
            //strncpy(name,_n,strlen(_n));
            name[0]='\0';
            strcpy(name,_n);
        }
        Father()
            :varPri(0),varPro(0),varPub(0)
        {
            name[0]='\0';
            strcpy(name,"undef");
        }
};

class Child:public Father
{
    private:
        char name[1024];
    protected:
    public:
        int varPri;
        int varPro;
        int varPub;
        Child(int _v1,int _v2,int _v3,const char * _n):varPri(_v1),varPro(_v2),varPub(_v3)
        {
            name[0]='\0';
            strncpy(name,_n,strlen(_n));
        }
};

int Print(class Father* c){
    char *buf=(char *)malloc(sizeof(char)*1024);
    bzero(buf,1024);
    c->toStr(buf,1024);
    printf("(%s)\n",buf);
    free(buf);
    return 0;
}

int main()
{
    Father *c=new Child(2,3,4,"child");
    Print(c);
    Father *f=new Father(1,2,3,"father");
    Print(f);
    c=new Child(3,4,5,"child");
    Print(c);
    Child cc=Child(5,6,7,"child");
    Print(&cc);
}
