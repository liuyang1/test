#include "leet.h"
// not limit time of transactions, but only have one share at any time.
// Just grab every rise
int maxProfit(int *prices, int pricesSize) {
    if (pricesSize <= 1) {
        return 0;
    }
    int i, profit;
    for (i = 1, profit = 0; i != pricesSize; i++) {
        profit += (prices[i] > prices[i - 1]) * (prices[i] - prices[i - 1]);
    }
    return profit;
}

#define CASE(a, e) {int r = maxProfit(a, sizeof(a) / sizeof(int)); \
                    showArr(a, sizeof(a) / sizeof(int)); \
                    printf("%d ?= %d %s\n", r, e, expect(r == e)); }

int main() {
    CASE(((int[]) {3, 2, 1}), 0);
    CASE(((int[]) {1, 2, 3}), 2);
    CASE(((int[]) {1, 2, 3, -1, 2}), 5);
    CASE(((int[]) {1, 2, 3, -1, 3}), 6);
    CASE(((int[]) {1, 2, 3, -1, 5, 7, -8, -1}), 17);
    return 0;
}
