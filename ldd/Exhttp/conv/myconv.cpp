#include "myconv.h"
#include <string.h>
#include <stdio.h>

#define GB2312NAME	"gb2312"
#define GBKNAME		"gbk"
#define BIG5NAME	"big5"
#define UTF8NAME	"utf-8"

#define METHOD_N	4

struct dect{
	int (*func)(char *);
	char *name;
	int maxoffset;
};

struct dect dectMethod[METHOD_N];

// 小端情况
#define SMALL_ENDIAN

int isGB2312( char *p)
{
#ifdef SMALL_ENDIAN
	unsigned char c1=*p;
	unsigned char c2=*(p+1);
#else
	unsigned char c1=*(p+1);
	unsigned char c2=*p;
#endif
	return (c1>=0xB0 && c1<=0xF7 && c2>=0xA0 && c2<=0xFE)? 2: 0;
}

int isGBK( char *p){
#ifdef SMALL_ENDIAN
	unsigned char c1=*p;
	unsigned char c2=*(p+1);
#else
	unsigned char c1=*(p+1);
	unsigned char c2=*p;
#endif
	return (c1>=0x81 && c1<=0xFE && c2>=0x40 && c2<=0xFE)? 2: 0;
}

int isBig5( char *p){
#ifdef SMALL_ENDIAN
	unsigned char c1=*p;
	unsigned char c2=*(p+1);
#else
	unsigned char c1=*(p+1);
	unsigned char c2=*p;
#endif
	return (c1>=0x81 && c1<=0xFE && ( ( c2>=0x40 && c2<=0x7E) || (c2>=0xA1 && c2<=0xFE)))? 2: 0;
}

int isEn( char *p){
	return 	*p>>7 ? 0 : 1;
}


int isUTF8(char *p){
	unsigned char c1=*p;
	unsigned char c2=*(p+1);
	unsigned char c3=*(p+2);
	if(c1<0x80)
		return 1;
	if(c1<0xC0)
		return 0;	// invalid utf8 encode
	if(c1<0xE0)		// 2 size utf8
	{
		if(c2&0xC0!=0x80)	// begin with 0x10...
		{
			return 0;
		}else{
			return 2;
		}
	}
	if(c1<0xF0)		// 3 size utf8
	{
		if((c2&0xC0)!=0x80||(c3&0xC0)!=0x80)
			return 0;
		else
			return 3;
	}
	return 0;
}

int init_get_encoding(){
	dectMethod[0].func=isUTF8;
	dectMethod[0].name=UTF8NAME;
	dectMethod[0].maxoffset=3;

	dectMethod[1].func=isGB2312;
	dectMethod[1].name=GB2312NAME;
	dectMethod[1].maxoffset=2;

	dectMethod[2].func=isGBK;
	dectMethod[2].name=GBKNAME;
	dectMethod[2].maxoffset=2;

	dectMethod[3].func=isBig5;
	dectMethod[3].name=BIG5NAME;
	dectMethod[3].maxoffset=2;
	return 0;
}

// !注意,不可以对每个检测方法下测试不同的偏移量,
// 而是对不同的偏移量依次尝试不同的检测方法,
// 不然可能会有无检测问题
int my_get_encoding(char *p,int len,char *encode){
	init_get_encoding();
	int i,j;
	char *pf;
	int ret;
	pf=p;
	if(isEn(pf)){
		pf++;
		while(isEn(pf)&&*p!='\0'){
			pf++;
		}
		if(*p=='\0'){
			strcpy(encode,"english");
			return 0;
		}
		for(j=0;j<METHOD_N;j++){
			ret=dectMethod[j].func(pf);
			//printf("offset %d,dect method %s ret %d\n",0,dectMethod[j].name,ret);
			if(ret!=0){
				strcpy(encode,dectMethod[j].name);
				return 0;
			}
		}
	}else{
		for(i=0;i!=3;i++){
			pf=p+i;
			for(j=0;j<METHOD_N;j++){
				if(i>=dectMethod[j].maxoffset)
					continue;
				while((ret=dectMethod[j].func(pf))){
					if(ret==0)
						break;
					pf+=ret;
					if(pf-p>=len){
						strcpy(encode,dectMethod[j].name);
						return 0;
					}
				}
				//printf("offset %d,dect method %s ret %d\n",i,dectMethod[j].name,ret);
			}
		}
	}

	strcpy(encode,"error");
	return -1;
}
