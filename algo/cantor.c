/** Cantor 变换与逆变换
 *  将序列转换为一个数值，（并将其逆变换回来）
 */

#include <stdio.h>
#include <stdlib.h>

int factorial(int n) {
    int p = 1, i;
    for (i = 1; i <= n; i++) {
        int t = p * i;
        if (t < p || t < i) {
            printf("detect overflow: %d * %d = %d\n", p, i, t);
            return -1; // overflow
        }
        p = t;
    }
    return p;
}

int cantor(int *arr, int size) {
    // arr[i] (- [1, size]
    int i;
    for (i = 0; i != size; i++) {
        if (arr[i] <= 0 || arr[i] > size) {
            return -1; // invalid
        }
    }
    int weight = factorial(size - 1);
    if (weight < 0) {
        return -1;
    }
    // cantor transfter
    int sum = 0, j;
    for (i = 0; i != size; i++) {
        int cnt = 0; // counter on predecessor which less than arr[i]
        for (j = 0; j != i; j++) {
            cnt += arr[j] < arr[i];
        }
        sum += (arr[i] - 1 - cnt) * weight;
        if (i == size - 1) {
            break;
        }
        weight /= (size - 1 - i);
    }
    // sum <= factorial(size)
    return sum;
}

void showArr(int *a, int size) {
    int i;
    printf("[");
    for (i = 0; i != size; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", a[i]);
    }
    printf("]\n");
}

int *decantor(int x, int size) {
    int *arr = malloc(sizeof(int) * size);
    int weight = factorial(size - 1);
    if (weight < 0) {
        return NULL;
    }
    int i, j;
    x--;
    for (i = 0; i != size; i++) {
        printf("%d\n", x / weight);
        arr[i] = x / weight + 1;
        int counter = 0;
        for (j = 0; j != i; j++) {
            counter += arr[j] >= arr[i];
        }
        printf("counter=%d\n", counter);
        arr[i] += counter;
        if (i == size - 1) {
            break;
        }
        x %= weight;
        weight /= (size - 1 - i);
    }
    return arr;
}

void test_factorial() {
    int i;
    for (i = 1; i != 20; i++) {
        int v = factorial(i);
        printf("factorial(%d) = %d\n", i, v);
        if (v < 0) {
            break;
        }
    }
}

int test_cantor(int *a, int size, int x) {
    int r = cantor(a, size);
    if (r != x) {
        return -1;
    }
    int *b = decantor(x, size);
    if (b == NULL) {
        return -1;
    }
    int i;
    r = 0;
    for (i = 0; i != size; i++) {
        if (a[i] != b[i]) {
            r = -1;
            break;
        }
    }
    free(b);
    return r;
}

int main() {
    test_factorial();
    // int arr[] = {3, 5, 7, 4, 1, 2, 9, 6, 8};
    // test_cantor(arr, sizeof(arr) / sizeof(arr[0]), 98884);
    int *a = decantor(19, 4);
    showArr(a, 4);
    free(a);
    a = decantor(96, 5);
    showArr(a, 5);
    return 0;
}
