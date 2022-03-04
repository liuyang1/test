/** https://codechef.com/porblems/COINS */

#include <stdint.h>
#include <stdio.h>

#define N	(100 * 1000 * 1000 + 1)
uint64_t coins_tbl[N]; // 0

// init one time
void coins_init() {
	uint64_t i = 0;
	coins_tbl[i] = 0;
	for (i = 1; i != N; i++) {
		uint64_t exchange = coins_tbl[i / 2] + coins_tbl[i / 3] + coins_tbl[i / 4];
		coins_tbl[i] = (i > exchange) ? i : exchange;
	}
	// printf("coins init done\n");
}

uint64_t coins_get(uint64_t x) {
	if (x == 0) {
		return 0;
	}
	if (coins_tbl[x] != 0) { // dynamic programming
		return coins_tbl[x];
	}
	uint64_t exchange = coins_get(x / 2) + coins_get(x / 3) + coins_get(x / 4);
	coins_tbl[x] = (x > exchange) ? x : exchange;
	return coins_tbl[x];
}

int chef_main() {
	// coins_init();
	uint64_t x;
	printf("xs = [\n");
	while (scanf("%zu", &x) == 1) {
		if (x >= N - 1) {
			printf("out of range %zu\n", x);
			continue;
		}
		printf("\t(%zu, %zu),\n", x, coins_get(x));
	}
	printf("]\n");
	return 0;
}

int main() {

	return chef_main();
	coins_init();
	uint64_t x;
	printf("xs = [\n");
	for (x = 0; x != N; x++) {
		printf("\t(%zu,%zu),\n", x, coins_get(x));
	}
	printf("]\n");
	return 0;
}
