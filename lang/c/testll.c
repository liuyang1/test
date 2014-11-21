#include <stdio.h>

unsigned long long num=0x1234123412341234ll;

int main(){
	printf("%x,%llx,%llu\n",num,num,num);
	printf("%llx\n",num);
	printf("%llx\n",num>>4);
	printf("%llx\n",(long long)0xFF<<24);
	printf("%llx\n",(long long)0x1F<<24);
	printf("!!%llx\n",(long long)(0xFF<<24));
	printf("%llx\n",(long long)(0x1F<<24));
	printf("%llx\n",(long long)((unsigned char)0xFF)<<24);
	printf("%llx\n",(long long)(0xFF>>4));
	printf("%llx\n",(long long)0xFF>>4);
	return 0;
}
