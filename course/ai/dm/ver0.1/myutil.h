#ifndef _MYUTIL_H_
#define _MYUTIL_H_

#define BUF_LEN		1024

#define CHK_NULL(p,str)\
	if(p==NULL){\
		printf(str);\
		exit(-1);\
	}

#define CHK_NEG(ret,str)\
	if(ret<0){\
		printf(#str);\
		exit(-1);\
	}

#define PROFILE_F_FILE		"./data/profile_f.txt"
#define PROFILE_M_FILE		"./data/profile_m.txt"
#define TEST_FILE			"./data/test.txt"
#define TRAIN_FILE			"./data/train.txt"

inline int double_bigger(const void *n1,const void *n2){
	return (*(double*)n1 > *(double*)n2)	?
		1	:	-1;
}
inline int int_bigger(const void *n1,const void *n2){
	return (*(int*)n1 > *(int*)n2)	?
		1	:	-1;
}
#define MALLOCTEST	{printf("malloc %d space\n",n);}

#endif
