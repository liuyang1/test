#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void *checkBuf(void *ptr, size_t offset, size_t *pLen, size_t elemSize) {
    if (offset < *pLen) {
        return ptr;
    }
    *pLen *= 2;
    void *p = realloc(ptr, elemSize * (*pLen));
    if (p == NULL) {
        free(ptr);
        printf("No memory\n");
        abort();
    }
    return p;
}

uint64_t *factor(uint64_t num, size_t *pSize) {
    if (num <= 1) {
        *pSize = 0;
        return NULL;
    }
    size_t len = 4, i = 0;
    uint64_t *arr = malloc(sizeof(uint64_t) * len);
    for (int f = 2; num != 1;) {
        // speed up #1
        if (f * f > num) {
            arr = checkBuf(arr, i + 1, &len, sizeof(uint64_t));
            arr[i++] = num;
            break;
        }
        if (num % f == 0) {
            arr = checkBuf(arr, i + 1, &len, sizeof(uint64_t));
            arr[i++] = f;
            num /= f;
        } else {
            // speed up #2
            if (f == 2) {
                f++;
            } else {
                f += 2;
            }
        }
    }
    *pSize = i;
    return arr;
}

int process(uint64_t num) {
    size_t size = 0;
    uint64_t *arr = factor(num, &size);

    printf("%lu:", num);
    for (size_t i = 0; i != size; i++) {
        printf(" %lu", arr[i]);
    }
    printf("\n");

    if (arr != NULL) {
        free(arr);
    }
    return 0;
}

int main(int argc, char **argv) {
    uint64_t num = 0;
    if (argc == 1) {
        while (scanf("%lu", &num)) {
            process(num);
        }
    } else {
        int i;
        for (i = 1; i != argc; i++) {
            sscanf(argv[i], "%lu", &num);
            process(num);
        }
    }
    return 0;
}
