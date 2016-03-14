#include "leet.h"
/*
 * one loop to update min
 * one more loop from backward to get max
 * get max profit of max - min array
 */
int maxProfit(int *prices, int pricesSize) {
    if (pricesSize <= 1) {
        return 0;
    }
    int *t = malloc(sizeof(int) * pricesSize);
    int i, min, max;
    for (i = 0, min = prices[0]; i != pricesSize; i++) {
        if (min > prices[i]) {
            min = prices[i];
        }
        t[i] = min;
    }
    int ret = 0, profit = 0;
    for (i = pricesSize - 1, max = prices[i]; i >= 0; i--) {
        if (max < prices[i]) {
            max = prices[i];
        }
        profit = max - t[i];
        if (profit > ret) {
            ret = profit;
        }
    }
    free(t);
    return ret;
}

#define CASE(a, e) {int r = maxProfit(a, sizeof(a) / sizeof(int)); \
                    showArr(a, sizeof(a) / sizeof(int)); \
                    printf("%d ?= %d %s\n", r, e, expect(r == e)); }

int main() {
    CASE(((int[]) {3, 2, 1}), 0);
    CASE(((int[]) {1, 2, 3}), 2);
    CASE(((int[]) {1, 2, 3, -1, 2}), 3);
    CASE(((int[]) {1, 2, 3, -1, 3}), 4);
    CASE(((int[]) {1, 2, 3, -1, 5, 7, -8, -1}), 8);
    return 0;
}
