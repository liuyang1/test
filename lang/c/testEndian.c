#include <stdio.h>
#include <endian.h>

int main(){
	printf("big: %d\nlitte: %d\nuse: %d\n",__BIG_ENDIAN,__LITTLE_ENDIAN,__BYTE_ORDER);
	return 0;
}
