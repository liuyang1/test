#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>

#define PI 3.1415926
#define EE 2.7182818

int main()
{
    const size_t ch = 2; // 2ch
    const size_t freq_lst[] = {320};
    const size_t rate = 48000; // 48kHz
    const size_t t = 10; // seconds
    // int16_t
    const char fn[] = "1.pcm";
    FILE *fp = fopen(fn, "wb");

    size_t k;
    for (k = 0; k != sizeof(freq_lst) / sizeof(freq_lst[0]); k++) {
        size_t freq = 48000 / freq_lst[k];
        size_t i, j;
        size_t m = t * rate;
        for (i = 0; i != t * rate; i++) {
            double x = sin(2.0 * PI * i / freq); // [-1, 1], sin wave
            // double a = (i + 1.) / m; // (0+, 1], min value is 1/m
            // double b = a * 10.397 - 10.397; // (-10.397+, 0], 10.397=log(INT16_MAX)=log(32767)
            // double c = exp(b); // (0+, 1]
            double z = x * INT16_MAX; // [-INT16_MAX,INT16_MAX], dbfs is linear line
                                          // dbfs = 20*log10(sqrt(mean(rms)))+3.0103
                                          // y=20*log10(x), y=exp(y), so the amplitude should range from [1, INT16_MAX] with exp line
            int16_t y = (int16_t)(z); // round
            if (y > 0) {
                y = -y;
            }
            // printf("x=%.3f z=%.3f y=%d\n", x, z, y);
            // printf("%f %f %f %f %f %d\n", x, a, b, c, z, y);
            for (j = 0; j != ch; j++) {
                fwrite(&y, sizeof(y), 1, fp);
            }
        }
    }
    fclose(fp);
}

