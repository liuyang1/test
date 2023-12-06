#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SIZE 0xDDEE
int main() {
	void *p = malloc(SIZE);
	memset(p, 0xCC, SIZE);
	printf("alloc size=%d\n", SIZE);
	strcpy(p, "hello");
	printf("%s\n", p);
	free(p);
	return 0;
}
