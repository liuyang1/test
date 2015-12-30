/* A Linear Time Majority Vote Algorithm
 * ref: http://www.cs.utexas.edu/~moore/best-ideas/mjrty/
 *
 * This simple algorithm, find majority just ONE pass.
 * > Awesome!
 */
// Warning:
// This function only works when ensure there's one element are majority.
// If we cannot ensure this, must count return value again to make sure it's
// majority.
int majorityElement(int* nums, int numsSize) {
    int v, c, i;
    for (i = 0, c = 0; i != numsSize; i++) {
        // if c == 0, select this one as majority
        if (c == 0) {
            c = 1;
            v = nums[i];
        } else if (nums[i] == v) { // if it repeat, then increase counter
            c++;
        } else { // if not repeat(occur other value), then decrease counter
            c--;
        }
    }
    return v;
}

#define CASE(a, e) {int r = majorityElement(a, sizeof(a) / sizeof(int)); \
    printf("%d ?= %d %s\n", r, e, r == e ? "PASS" : "fail"); }
int main() {
    CASE(((int []){1}), 1);
    CASE(((int []){3, 2, 3}), 3);
    CASE(((int []){1, 1, 1, 2, 2, 3, 3, 2, 2, 2, 3, 2, 2}), 2);
    return 0;
}
