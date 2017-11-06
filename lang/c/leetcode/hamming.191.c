#include <stdio.h>
#include <stdint.h>

int hammingWeight(uint32_t n) {
    int cnt;
    for (cnt = 0; n != 0; n &= n - 1, cnt++) {
    }
    return cnt;
}

/* ref: https://en.wikipedia.org/wiki/Hamming_weight#Efficient_implementation
 */
int hammingWeight_count(uint32_t n) {
    const uint64_t m1  = 0x55555555; //binary: 0101...
    const uint64_t m2  = 0x33333333; //binary: 00110011..
    const uint64_t m4  = 0x0f0f0f0f; //binary:  4 zeros,  4 ones ...
    const uint64_t m8  = 0x00ff00ff; //binary:  8 zeros,  8 ones ...
    const uint64_t m16 = 0x0000ffff; //binary: 16 zeros, 16 ones ...
    n = (n & m1) + ((n >>  1) & m1);   //put count of each  2 bits into those  2 bits
    n = (n & m2) + ((n >>  2) & m2);   //put count of each  4 bits into those  4 bits
    n = (n & m4) + ((n >>  4) & m4);   //put count of each  8 bits into those  8 bits
    n = (n & m8) + ((n >>  8) & m8);   //put count of each 16 bits into those 16 bits
    n = (n & m16) + ((n >> 16) & m16); //put count of each 32 bits into those 32 bits
    return n;
}

int hammingDistance(uint32_t a, uint32_t b) {
    return hammingWeight(a ^ b);
}

int main() {
    int i;
    for (i = 0; i != 16; i++) {
        printf("hammingWeight(%2d 0x%x) = %d %d\n",
               i, i, hammingWeight(i), hammingWeight_count(i));
    }

    uint32_t a = 4, b = 0xf;
    printf("hammingDistance(%d, %d) = %d\n", a, b, hammingDistance(a, b));
    return 0;
}
