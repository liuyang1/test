#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int counter = 0;
int loops;

void *worker(void *data) {
	int i;
	for (i = 0; i != loops; i++) {
		counter++;
	}
	return NULL;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Usage: %s counter\n", argv[0]);
		return -1;
	}
	loops = atoi(argv[1]);

	pthread_t t[2];
	pthread_create(t, NULL, worker, (void *)0);
	pthread_create(t + 1, NULL, worker, (void *)1);
	pthread_join(t[0], NULL);
	pthread_join(t[1], NULL);

	printf("final %d\n", counter);

	printf("%s\n", counter == 2 * loops ? "PASS" : "FAIL");
	return 0;
}
