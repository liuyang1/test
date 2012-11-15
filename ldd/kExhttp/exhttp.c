#include "exhttp.h"
#include "conv.h"
#include <linux/string.h>
#include <linux/ctype.h>

int parse_html_seg(char* input,int inputlen,char* output,int outputlen){
	char* loc1=strchr(input,(int)'<');
	char* loc2=strchr(input,(int)'>');
	char *pi;
	char *po;
	// handle loss '<' problem
	if(loc1>loc2){
		inputlen=inputlen-(loc2-input)-1;
		input=loc2+1;
	}
	pi=input;//float ptr of input
	po=output;
	
	// filter content between '<' and '>'
	while(pi-input<=inputlen && po-output<=outputlen){
		// find '<'
		loc1=strchr(pi,(int)('<'));
		if(loc1!=NULL){
			loc2=strchr(loc1,(int)('>'));
			if(loc2!=NULL){					
				// '<','>',
				// copy from now offset to pos of '<' 
				// and skip content between '<' and '>'
				memcpy(po,pi,loc1-pi);
				inputlen-=loc2-pi+1;
				po+=loc1-pi;
				pi=loc2+1;
			}else{
				// '<',not exist '>',
				// direclty copy from now offset to pos of '<'
				memcpy(po,pi,loc1-pi);
				po+=loc1-pi;
				break;
			}
		}else{
			// isn't exist '<', 
			// directly to copy
			memcpy(po,pi,inputlen);
			po+=inputlen;
			break;
		}
	}
	
	return po-output;
}

int filter_space(char* in,int inlen,char* out,int outlen){
	char *pi=in,*po=out;
	while(pi-in<inlen && *pi!='\0'){
		if(isspace((int)(*pi))){
			while(isspace((int)(*pi)))
				pi++;
			*po++=' ';
		}else{
			*po++=*pi++;
		}
	}
	*po++='\0';
	return po-out;
}

