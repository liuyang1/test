#include <stdio.h>
#include "fsm.h"
#define BUFLEN	1024

int main(int args,char** argv){
	if(args!=2){
		printf("usage: ngrep target-filename src-filename\n");
		return -1;
	}

	FILE *fp=fopen(argv[1],"r");
	if(fp==NULL){
		printf("there isn't exist %s file\n",argv[1]);
		return -2;
	}

	//char tarbuf[BUFLEN];
	//while(!feof(fp)){
	//	fgets(tarbuf,1024,fp);
	//	puts(tarbuf);
	//}

	char tar[]="liuyang1";
	char text[]="shabiliuyang1dashabi";

	fsm f;
	f.addStr(tar);
	f.chkTxt(text);

	return 0;
}
