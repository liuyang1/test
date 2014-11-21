#include <stdio.h>
#include <locale.h>

int main(){
	if(!setlocale(LC_CTYPE,"")){
		fprintf(stderr,"no locale\n");
		return 1;
	}
	printf("%ls",L"嗨,世界\n");
	printf("%s","hello,world\n");
	char buf[1024];
	printf(">>");
	scanf("%ls",buf);
	printf("%ls\n",(buf));
	return 0;
}
