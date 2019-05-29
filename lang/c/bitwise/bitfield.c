#include <stdio.h>
#include <stdint.h>

struct st {
	uint32_t cmd: 23; // other part
	uint32_t size: 9; // [0,511]
};

int main() {
	printf("%lu\n", sizeof(struct st));
	struct st a = {
		.size = 128,
		.cmd = 0xff,
	};
	printf("a=%08x size=%x cmd=%x\n", a, a.size, a.cmd);
	return 0;
}
