#include "myconv.h"
#include <string.h>

int isGB2312(char *p)
{
	return 1;
}
int isGBK(char *p){
	return 1;
}

int isBig5(char *p){
	return 1;
}

int isEn(char *p){
	return 	*p>>7 ? 1 : 0;
}
