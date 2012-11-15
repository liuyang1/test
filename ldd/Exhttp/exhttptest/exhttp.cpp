#include "exhttp.h"
#include "conv.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void warn_printf(char * str){
#ifdef _debug_
	printf("[WARNING]: %s\n",str);
#endif
}



int parse_html_seg(char* input,int inputlen,char* output,int outputlen){
	char* loc1=strchr(input,(int)'<');
	char* loc2=strchr(input,(int)'>');
	if(loc1>loc2){
//		printf("handle > problem!\n");
		inputlen=inputlen-(loc2-input)-1;
		input=loc2+1;
	}
	char *pi=input;//float ptr of input
	char *po=output;
/*	
	while(pi-input<=inputlen){
		while(*pi!='<' && pi-input<=inputlen){
			*po++=*pi++;
		}
		while(*pi++!='>'&& pi-input<=inputlen){
			;
		}
	}
*/	
	
	while(pi-input<=inputlen && po-output<=outputlen){
		loc1=strchr(pi,int('<'));
		if(loc1!=NULL){
			loc2=strchr(loc1,int('>'));
			if(loc2!=NULL){
				memcpy(po,pi,loc1-pi);
				inputlen-=loc2-pi+1;
				po+=loc1-pi;
				pi=loc2+1;
			}else{
				memcpy(po,pi,loc1-pi);
				po+=loc1-pi;
				break;
			}
		}else{
			memcpy(po,pi,inputlen);
			po+=inputlen;
			break;
		}
	}
	
	/*
	while(pi-input<=inputlen && po-output<=outputlen){
		if(isInBracket==0){
			if(*pi=='<'){
				isInBracket=1;
				pi++;
				continue;
			}else if(*pi=='>'){
				warn_printf("two > or no < direct >");
				*output='\0';
				return -1;
			}else{
				*po=*pi;
				po++;
				pi++;
				continue;
			}
		}else{
			if(*pi=='>'){
				isInBracket=0;
				pi++;
				continue;
			}else{
				pi++;
			}
		}
	}
	*/
	return po-output;
}

int filter_space(char* in,int inlen,char* out,int outlen){
	char *pi=in,*po=out;
	while(pi-in<inlen && *pi!='\0'){
		if(isspace(int(*pi))){
			while(isspace(int(*pi)))
				pi++;
			*po++=' ';
		}else{
			*po++=*pi++;
		}
	}
	*po++='\0';
	return po-out;
}

#ifdef _debug_
int main(int argc,char** argv){
	if(argc<2){
		printf("Usage: exhttp filename\n");
		return 0;
	}
	FILE *fp=fopen(argv[1],"r");
	if(!fp){
		printf("could not open file\n");
		return 0;
	}
#ifdef _TEST
for(int i=0;i<100000;i++){
#endif
	char buf[2048]={0};
	//scanf("%s",buf);
	while(!feof(fp)){
		//fgets(buf,2048,fp);
		fread(buf,sizeof(char),2048,fp);
		char out[2048];

		int ret;
#ifndef _TEST
		printf("\n/******************************************/\n");
		printf("in : %s\n",buf);
#endif

		ret=parse_html_seg(buf,strlen(buf),out,2048);
		int ret2=filter_space(out,ret,buf,2048);
#ifndef _TEST
		printf("\n\nfilt %s\n",buf);
		printf("len: %d\n",ret2);
#endif
		int ret3=ConvEx(buf,ret2,out,2048);
#ifndef _TEST
		printf("\n\nconv %s\n",out);
#endif
		if(ret==-1){
			goto EXIT;
		}
	}
#ifdef _TEST
	rewind(fp);
	if(i%10000==9999)
		printf("%d\n",i/10000);
}
#endif
EXIT:
fclose(fp);
return 0;
}
#endif
