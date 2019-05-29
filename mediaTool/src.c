#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef struct {
    uint32_t in_rate;
    uint32_t out_rate;
    uint32_t in_size;
    uint32_t out_size;
    uint32_t in_acc;
    uint32_t out_acc;
} src_t;

size_t src_memReq() {
    return sizeof(src_t);
}

int src_set(void *ctx, uint32_t in_rate, uint32_t out_rate,
            uint32_t in_size, uint32_t out_size) {
    src_t *p = (src_t *)ctx;
    p->in_rate = in_rate;
    p->out_rate = out_rate;
    p->in_size = in_size;
    p->out_size = out_size;
    p->in_acc = p->out_acc = 0;
    return 0;
}

uint32_t src_conv(void *ctx,
                  void *input, uint32_t in_frame,
                  void *output, uint32_t out_frame) {
    src_t *p = (src_t *)ctx;
    assert(out_frame * p->in_rate == in_frame * p->out_rate);
    uint32_t i;
    for (i = 0; i != out_frame; i++) {
        uint32_t t = i * p->in_rate;
        uint32_t j = t / p->out_rate;
        uint32_t k = t % p->out_rate;
        // TODO: support 16bit and other audio format?
        double *o = (double *)output;
        double *in = (double *)input;
        o[i] = in[j] + (in[j + 1] - in[j]) * k / (p->out_rate + 0.0);
    }
    return out_frame;
}

void show_arr(double *a, size_t n) {
    printf("[");
    size_t i;
    for (i = 0; i != n; i++) {
        printf("%.4f, ", a[i]);
    }
    printf("]\n");
}

int unit(uint32_t input_rate, uint32_t input_len,
         uint32_t output_rate, uint32_t output_len) {
    assert(output_rate * input_len == input_rate * output_len);
    size_t sz = src_memReq();
    void *p = malloc(sz);

    src_set(p, input_rate, output_rate, sizeof(double), sizeof(double));

    double *input = malloc(sizeof(double) * input_len);
    double *output = malloc(sizeof(double) * output_len);
    uint32_t i;
    for (i = 0; i != input_len; i++) {
        input[i] = sin(i / 20.);
    }

    src_conv(p, (void *)input, input_len, (void *)output, output_len);

    show_arr(input, input_len);
    show_arr(output, output_len);

    free(p);
    free(input);
    free(output);
    return 0;
}

int main() {
    unit(3, 120, 2, 80);
    unit(2, 80, 3, 120);
    unit(11, 110, 12, 120);
    return 0;
}
