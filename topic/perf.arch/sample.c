#include <stdio.h>

void showArrF(float *a, size_t n) {
    size_t i;
    for (i = 0; i != n; i++) {
        printf("%7.4f,", a[i]);
    }
    printf("\n");
}

void showArrI(int *a, size_t n) {
    size_t i;
    for (i = 0; i != n; i++) {
        printf("%2d,", a[i]);
    }
    printf("\n");
}

#include <immintrin.h> // avx header file
// search functon from toolchain's header file
#if 0
int test_mul_float() {
    float a[] = {0, 1, 2, 3, 4, 5, 6, 7};
    float b[] = {7, 6, 5, 4, 3, 2, 1, 0};
    float c[] = {0, 0, 0, 0, 0, 0, 0, 0};

    __m256 ymm0, ymm1, zmm0;
    ymm0 = __builtin_ia32_loadups256(a);
    ymm1 = __builtin_ia32_loadups256(b);

    zmm0 = __builtin_ia32_mulps256(ymm0, ymm1);

    __builtin_ia32_storeups256(c, zmm0);
    showArrF(a, 8);
    showArrF(b, 8);
    showArrF(c, 8);

    return 0;
}
#endif

int test_mul_int() {
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7};
    int b[] = {7, 6, 5, 4, 3, 2, 1, 0};
    int c[] = {0, 0, 0, 0, 0, 0, 0, 0};

    __m128i ymm0, ymm1, zmm0;
    ymm0 = __builtin_ia32_loaddqu(a);
    ymm1 = __builtin_ia32_loaddqu(b);

    zmm0 = __builtin_ia32_pmuludq(ymm0, ymm1);

    __builtin_ia32_storedqu(c, zmm0);
    showArrI(a, 8);
    showArrI(b, 8);
    showArrI(c, 8);

    return 0;

}

int main() {
    test_mul_int();
}
