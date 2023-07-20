#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *inc_bracket(char *in, size_t n)
{
	n = 2 * (n + 1);
	char *p = malloc(n + 1);
	snprintf(p, n + 1, "(%s)", in);
	return p;
}
void free_arr(char **p, size_t n)
{
	size_t i;

	for (i = 0; i != n; i++)
		free(p[i]);
	free(p);
}
char **bracket(size_t n, size_t *out_num)
{
	if (n <= 0) {
		char **p = malloc(sizeof(char *) * 1);
		p[0] = strdup("");
		*out_num = 1;
		return p;
	}
	size_t num = 0;
	char **acc = NULL;
	size_t m;
	for (m = 0; m != n; m++) {
		size_t fst_num, rst_num;
		char **fst = bracket(m, &fst_num);
		char **rst = bracket(n - m - 1, &rst_num);

		size_t prd_num = fst_num * rst_num;
		char **prd = malloc(sizeof(char *) * prd_num);

		size_t i, j, k;
		for (i = k = 0; i != fst_num; i++) {
			char *more = inc_bracket(fst[i], m);
			for (j = 0; j != rst_num; j++) {
				char *r = malloc(sizeof(char) * (2 * n + 1));
				snprintf(r, 2 * n + 1, "%s%s", more, rst[j]);
				prd[k++] = r;
			}
			free(more);
		}

		acc = realloc(acc, sizeof(char *) * (num + prd_num));
		memcpy(acc + num, prd, sizeof(char *) * prd_num);
		num += prd_num;

		free_arr(fst, fst_num);
		free_arr(rst, rst_num);
		free(prd);
	}
	*out_num = num;
	return acc;
}
int unit(size_t n)
{
	size_t num;
	char **p = bracket(n, &num);

	printf("%zu: %zu\n", n, num);
	size_t i;
	for (i = 0; i != num; i++)
		printf("'%s', ", p[i]);
	printf("\n");
	free_arr(p, num);
	return num;
}
int main()
{
	unit(0);
	unit(1);
	unit(2);
	unit(3);
	unit(4);
	unit(5);
	unit(6);
	return 0;
}
