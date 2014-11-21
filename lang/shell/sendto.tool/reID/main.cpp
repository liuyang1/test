/*
 * =================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  release all streamid
 *
 *        Version:  1.0
 *        Created:  08/16/12 10:14:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *        Company:  auto@ustc
 *
 * =================================================================
 */

#include "DataInfo.h"

typedef union{
	uint64_t id64;
	struct{
		uint16_t spi;
		uint16_t freq;
		uint16_t streamtype;
		uint16_t iptype;
	}s;
}ID;
int main(){
	ID id;
	id.s.spi=8;
	for(int i=0;i<24;i++){
		for(int j=0;j<2;j++){
			for(int k=0;k<5;k++){
				id.s.freq=i;
				id.s.streamtype=j;
				id.s.iptype=k;
				printf("streamid: %lX\n",id.id64);
				Free_Stream(id.id64);
			}
		}
	}
	return 0;
}
