// gcc 1.c -msse4.1
#include <stdio.h>
// header files in /usr/lib/gcc/x86_64-linux-gnu/4.4/include
#include <x86intrin.h> // all sse/avx headers
#include <smmintrin.h> // -msse4.1
#include <immintrin.h> // avx, avx2, fma
                       // #include <zmmintrin.h> // avx512

int mul_epi32() {
    int arr[] = {3, 1, 4, 1,
        5, 9, 2, 6};
    __m128i a0 = _mm_load_si128((__m128i*)&arr[0]);
    __m128i a4 = _mm_load_si128((__m128i*)&arr[4]);
    __m128i mul = _mm_mullo_epi32(a0, a4);
    int res[4];
    _mm_store_si128((__m128i*)res, mul);
    printf("%d %d %d %d\n", res[0], res[1], res[2], res[3]);
    // 15, 9, 8, 6
}

int mul_8_epi32() {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6,
        6, 2, 9, 5, 1, 4, 1, 3};
    __m256i a0 = _mm256_load_si256((__m256i*)&arr[0]);
    __m256i a4 = _mm256_load_si256((__m256i*)&arr[8]);
    __m256i mul = _mm256_mullo_epi32(a0, a4);
    int res[8];
    _mm256_store_si256((__m256i*)res, mul);
    printf("%d %d %d %d - %d %d %d %d\n",
           res[0], res[1], res[2], res[3],
           res[4 + 0], res[4 + 1], res[4 + 2], res[4 + 3]);
    // 18, 2, 36, 5, 5, 36, 2, 18
}

int add_8_epi32() {
    int data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    __m256i vecData = _mm256_loadu_si256((__m256i*)data);

    // 将vecData的低128位和高128位分别相加
    __m256i vecSum = _mm256_add_epi32(vecData, _mm256_permute2x128_si256(vecData, vecData, 0x01));
    vecSum = _mm256_add_epi32(vecSum, _mm256_shuffle_epi32(vecSum, _MM_SHUFFLE(0, 0, 3, 2)));
    vecSum = _mm256_add_epi32(vecSum, _mm256_shuffle_epi32(vecSum, _MM_SHUFFLE(0, 0, 0, 1)));
    // 提取累加结果
    int result;
    _mm_storeu_si128((__m128i*)&result, _mm256_extracti128_si256(vecSum, 0));
    printf("Sum: %d\n", result);

    return 0;
}

int main() {
    mul_epi32();
    mul_8_epi32();
    add_8_epi32();
}
