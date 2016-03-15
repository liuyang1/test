#include "leet.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

// copy from stock.121.c
int maxProfit0(int *prices, int pricesSize) {
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

int maxProfit1(int *prices, int pricesSize) {
    if (pricesSize <= 1) {
        return 0;
    }
    int m = maxProfit0(prices, pricesSize);
    int i;
    for (i = 1; i < pricesSize - 1; i++) {
        // two transactions
        m = MAX(m,
                maxProfit0(prices, i) +
                maxProfit0(prices + i, pricesSize - i));
    }
    return m;
}

int skipSame(int *prices, int pricesSize, int v, int *idx) {
    int i;
    for (i = *idx; i != pricesSize; i++) {
        if (prices[i] != v) {
            break;
        }
    }
    *idx = i;
}
int *compress(int *prices, int pricesSize, int *size) {
    int *p = malloc(sizeof(int) * pricesSize);
    int i = 0, j = 0, sign;

    // init p[0];
    p[j++] = prices[i++];
    // init p[1] & sign;
    skipSame(prices, pricesSize, p[j - 1], &i);
    if (i == pricesSize) {
        goto end;
    }
    p[j++] = prices[i++];
    sign = p[j - 1] > p[j - 2];
    // search when have different sign
    for (; i != pricesSize; i++) {
        skipSame(prices, pricesSize, p[j - 1], &i);
        if (i == pricesSize) {
            goto end;
        }
        if ((prices[i] > p[j - 1]) == sign) {
            p[j - 1] = prices[i];
        } else {
            p[j++] = prices[i];
            sign = p[j - 1] > p[j - 2];
        }
    }
end:
    *size = j;
    return p;
}
// use compress to optimization
int maxProfit(int *prices, int pricesSize) {
    if (pricesSize <= 1) {
        return 0;
    }
    int sz, m;
    int *p = compress(prices, pricesSize, &sz);
    m = maxProfit1(p, sz);
    free(p);
    return m;
}

#define CASE(a, e) {int r = maxProfit(a, sizeof(a) / sizeof(int)); \
                    printf("%d ?= %d %s\n", r, e, expect(r == e)); }

int main() {
    CASE(((int[]) {}), 0);
    CASE(((int[]) {3}), 0);
    CASE(((int[]) {0, 3}), 3);
    CASE(((int[]) {3, 3}), 0);
    CASE(((int[]) {1, 2, 3, -1, 2}), 5);
    CASE(((int[]) {1, 2, 3, -1, 3}), 6);
    CASE(((int[]) {1, 2, 3, -1, 5, 7, -8, -1}), 15);
    CASE(((int[]) {2, 1, 2, 1, 0, 0, 1}), 2);
    CASE(((int[]) {1, 2, 3, -1, 2, 2, -1, 2, 3}), 7);
    CASE(((int[]) {1, 2, 3, 4, 5, 6, 7, 8}), 7);
    CASE(((int[]) {7, 6, 5, 4, 3, 2, 1, 0}), 0);
    return 0;
}
