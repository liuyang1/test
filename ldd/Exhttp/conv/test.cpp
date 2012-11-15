#include "myconv.h"
#include <stdio.h>
#include <string.h>

int main(){
	char c='a';
	printf("isEn: %x %d\n",c,isEn(&c));
	char utf[]=	{0xE6,0xB8,0xAC,0xE8,0xA9,0xA6,0x00};		// utf-8 用
	char big5[]={0xB4,0xFA,0xB8,0xD5,0x00};					// big5 测试
	char gbk[]=	{0x9C,0x79,0xD4,0x87,0x00};					// gbk 测试
	printf("big5: %2x %d\n",*big5,isBig5(big5));
	printf("gbk : %2x %d\n",*gbk,isGBK(gbk));
	printf("\tisutf8 %d\n",isUTF8(gbk));
	printf("utf8: %2x %d\n",*utf,isUTF8(utf));


	char encode[10];
	init_get_encoding();
	my_get_encoding(big5,strlen(big5),encode);
	printf("big5: %s\n",encode);
	my_get_encoding(gbk,strlen(gbk),encode);
	printf("gbk:  %s\n",encode);
	my_get_encoding(utf,strlen(utf),encode);
	printf("utf-8:%2x %s\n",*utf,encode);
	return 0;
}
