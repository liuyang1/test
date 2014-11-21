#ifndef _EXHTTP_H_
#define _EXHTTP_H_

// handle html segment
int parse_html_seg(char* input,const int inputlen,char* output,int outputlen);
// clean needless space
int filter_space(char* in,int inlen,char* out,int outlen);

#endif
