/** 这是从alsa-lib latency.c里面抄写的
 * bandpass filter sweep LFO的算法代码
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#define FILTERSWEEP_LFO_CENTER  2000.
#define FILTERSWEEP_LFO_DEPTH   1800.
#define FILTERSWEEP_LFO_FREQ    0.2
#define FILTER_BANDWIDTH        50


int channels;
float lfo, dlfo, fs, fc, BW, C, D, a0, a1, a2, b1, b2, *x[3], *y[3];

void initeffect(int rate, int chn) {
    fs = rate;
    BW = FILTER_BANDWIDTH;
    lfo = 0;
    dlfo = 2. * M_PI * FILTERSWEEP_LFO_FREQ / fs;
    channels = chn;

    x[0] = (float*)malloc(channels * sizeof(float));
    x[1] = (float*)malloc(channels * sizeof(float));
    x[2] = (float*)malloc(channels * sizeof(float));
    y[0] = (float*)malloc(channels * sizeof(float));
    y[1] = (float*)malloc(channels * sizeof(float));
    y[2] = (float*)malloc(channels * sizeof(float));
}

void deiniteffect() {
    free(x[0]);
    free(x[1]);
    free(x[2]);
    free(y[0]);
    free(y[1]);
    free(y[2]);
}

void applyeffect(void *buffer, int r) {
    int16_t *samples = buffer;
    int i, frames = r / sizeof(int16_t) / channels;
    for (i = 0; i < frames; i++) {
        int chn;

        // center freqence, 2k+-1.8k, [0.2k, 3.8k]
        fc = sin(lfo) * FILTERSWEEP_LFO_DEPTH + FILTERSWEEP_LFO_CENTER;
        lfo += dlfo;
        if (lfo > 2. * M_PI) {
            lfo -= 2. * M_PI;
        }

        C = 1. / tan(M_PI * BW / fs); // const?
        D = 2. * cos(2 * M_PI * fc / fs);

        a0 = 1. / (1. + C);
        a1 = 0;
        a2 = -a0;

        b1 = -C * D * a0;
        b2 = (C - 1) * a0;
#if 0
        printf("fc=%.2f lfo=%.3f C=%.2f D=%.3f a=%.2f,%.2f,%.2f b=%.2f,%.2f\n",
               fc, lfo, C, D, a0, a1, a2, b1, b2);
#endif

        for (chn = 0; chn != channels; chn++) {
#if 0
            x[chn][2] = x[chn][1];
            x[chn][1] = x[chn][0];

            y[chn][2] = y[chn][1];
            y[chn][1] = y[chn][0];
#else
            x[2][chn] = x[1][chn];
            x[1][chn] = x[0][chn];
            y[2][chn] = y[1][chn];
            y[1][chn] = y[0][chn];
#endif

            x[0][chn] = samples[i * channels + chn];
            y[0][chn] = a0 * x[0][chn] + a1 * x[1][chn] + a2 * x[2][chn]
                - b1 * y[1][chn] - b2 * y[2][chn];
            samples[i * channels + chn] = y[0][chn];
        }
    }
}

int main() {
    int rate = 48000, chn = 2;
    initeffect(rate, chn);

    // 200Hz sine wave
    int sig_rate = 2000;
    float k = 2 * M_PI * sig_rate / rate;
    int seconds = 10;
    size_t bytes = rate * sizeof(int16_t) * chn * seconds;
    int16_t *b = malloc(bytes);
    size_t i;
    for (i = 0; i != bytes / sizeof(int16_t) / chn; i++) {
        b[chn * i + 0] = b[chn * i + 1] = (32000 * sin(k * i));
    }

    FILE *fp = fopen("in.pcm", "w");
    fwrite(b, bytes, 1, fp);
    fclose(fp);

    applyeffect(b, bytes);

    fp = fopen("out.pcm", "w");
    fwrite(b, bytes, 1, fp);
    fclose(fp);
    deiniteffect();
    return 0;
}
