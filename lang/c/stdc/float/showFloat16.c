/** parse half-precision floating point number */
#include <stdint.h>
#include <stdio.h>

void showFloat16(int16_t x) {
    int sign = x >> 15;
    int exp = (x >> 10) & (0x1f); // 5bit
    unsigned int bits = x & 0x3ff; // 10bit
    printf("0x%4X: sign=%d exp=%d bits=0x%x\n", x, sign, exp, bits);

    int den = 1024;
    int frac;
    if (exp == 0) {
        if (bits == 0) {
            printf("f=%c0\n", sign == 0 ? '+' : '-');
            return;
        } else {
            exp -= 14;
            frac = bits;
        }
    } else if (exp == 0x1f) {
        if (bits == 0) {
            printf("f=%cInf\n", sign == 0 ? '+' : '-');
        } else {
            printf("f=NaN\n");
        }
        return;
    } else {
        exp -= 15;
        frac = den + bits;
    }
    frac *= sign == 0 ? 1 : -1;
    if (exp < 0) {
        do {
            den *= 2;
            exp++;
        } while (exp != 0);
    } else if (exp > 0) {
        do {
            if (den != 1) {
                den /= 2;
            } else {
                frac *= 2;
            }
            exp--;
        } while (exp != 0);
    }
    while (frac % 2 == 0 && den % 2 == 0) {
        frac /= 2;
        den /= 2;
    }
    printf("f=%d/%d=%f\n", frac, den, frac / (den + 0.));
}

int main() {
    printf("---- number in practice ----\n");
    showFloat16(0x3BFF);
    showFloat16(0x0676);
    showFloat16(0x08B5);
    showFloat16(0x0903);

    printf("---- examples           ----\n");
    showFloat16(0x0001); // smallest positive subnormal number
    showFloat16(0x03ff); // largest subnormal number
    showFloat16(0x0400); // smallet positive normal number
    showFloat16(0x7Bff); // larget normal number
    showFloat16(0x3Bff); // larget number less than one
    showFloat16(0x3C00); // one
    showFloat16(0x3C01); // smallet number larger than one
    showFloat16(0x3555); // 1/3

    printf("---- numberw with limit ----\n");
    showFloat16(0xC000); // -2
    showFloat16(0x0000); // 0
    showFloat16(0x8000); // -0
    showFloat16(0x7c00); // Inf
    showFloat16(0xfc00); // -Inf
    return 0;
}
