int isMajority(int *nums, int numsSize, int n) {
    int i, cnt;
    for (i = 0, cnt = 0; i != numsSize; i++) {
        if (nums[i] == n) {
            cnt++;
        }
    }
    return cnt > numsSize / 2;
}

int majorityElement(int* nums, int numsSize) {
    srand(0);
    while (1) {
        int guess = nums[rand() % numsSize];
        if (isMajority(nums, numsSize, guess)) {
            return guess;
        }
    }
}

#define CASE(a, e) {int r = majorityElement(a, sizeof(a) / sizeof(int)); \
    printf("%d ?= %d %s\n", r, e, r == e ? "PASS" : "fail"); }
int main() {
    CASE(((int []){1}), 1);
    CASE(((int []){3, 2, 3}), 3);
    return 0;
}
