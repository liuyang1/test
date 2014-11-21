#ifndef _MYCONV_H_
#define _MYCONV_H_

int isGB2312(char *p);

int isGBK(char *p);

int isBig5(char *p);

int isEn(char *p);

int isUTF8(char *p);

int init_get_encoding();

int my_get_encoding(char *p,int len,char *encode);
#endif
