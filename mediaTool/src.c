#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef struct {
    uint32_t in_rate;
    uint32_t out_rate;
    // Intialy, we want to design interface to support free contexting convert.
    // It need save these as context.
    // However, it's complicated.
    // With limit,
    //      assert(out_frame * p->in_rate == in_frame * p->out_rate);
    // it don't need these part now.
#if 0
    uint32_t in_size;
    uint32_t out_size;
    uint32_t in_acc;
    uint32_t out_acc;
#endif
} src_t;

size_t src_memReq() {
    return sizeof(src_t);
}

int src_set(void *ctx, uint32_t in_rate, uint32_t out_rate,
            uint32_t in_size, uint32_t out_size) {
    src_t *p = (src_t *)ctx;
    p->in_rate = in_rate;
    p->out_rate = out_rate;
#if 0
    p->in_size = in_size;
    p->out_size = out_size;
    p->in_acc = p->out_acc = 0;
#endif
    return 0;
}

uint32_t src_conv_double(void *ctx,
                         void *input, uint32_t in_frame,
                         void *output, uint32_t out_frame) {
    src_t *p = (src_t *)ctx;
    assert(out_frame * p->in_rate == in_frame * p->out_rate);
    uint32_t i;
    for (i = 0; i != out_frame; i++) {
        uint32_t t = i * p->in_rate;
        uint32_t j = t / p->out_rate;
        uint32_t k = t % p->out_rate;
        double *o = (double *)output;
        double *in = (double *)input;
        o[i] = in[j] + (in[j + 1] - in[j]) * k / (p->out_rate + 0.0);
    }
    return out_frame;
}

// int 16bit 2channel to int 16bit 2channel
uint32_t src_conv_int16(void *ctx,
                        void *input, uint32_t in_frame,
                        void *output, uint32_t out_frame) {
    src_t *p = (src_t *)ctx;
    assert(out_frame * p->in_rate == in_frame * p->out_rate);
    int16_t *o = (int16_t *)output;
    int16_t *in = (int16_t *)input;
    uint32_t i;
    for (i = 0; i != out_frame; i++) {
        uint32_t t = i * p->in_rate;
        uint32_t j = t / p->out_rate;
        // unify to signed number
        int32_t k = t % p->out_rate;
        int32_t den = p->out_rate;
        uint32_t u = 2 * j;
        uint32_t v = 2 * i;
        o[v] = in[u] + ((in[u + 2] - in[u]) * k) / den;
#if 1
        // overflow/mixing detect
        if (!((in[u] <= o[v] && o[v] <= in[u + 2]) ||
             (in[u] >= o[v] && o[v] >= in[u + 2]))) {
            printf("overflow: %hd %hd r=%d/%d => %hd\n",
                   in[u], in[u + 2], k, den, o[v]);
        }
#endif
#if 0
        if (in[u] != 0) {
            printf("debug: %hd %hd r=%d/%d => %hd\n",
                   in[u], in[u + 2], k, den, o[v]);
        }
#endif
        u++;
        v++;
        o[v] = in[u] + ((in[u + 2] - in[u]) * k) / den;
    }
    return out_frame;
}

void show_arr_double(double *a, size_t n) {
    printf("[");
    size_t i;
    for (i = 0; i != n; i++) {
        printf("%.4f, ", a[i]);
    }
    printf("]\n");
}

void show_arr_int16(int16_t *a, size_t n) {
    printf("[");
    size_t i;
    for (i = 0; i != n; i++) {
        printf("%hd:%hd, ", a[2 * i], a[2 * i + 1]);
        // printf("%hd, ", a[2 * i]);
    }
    printf("]\n");
}

int unit_double(uint32_t input_rate, uint32_t input_len,
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

    src_conv_double(p, (void *)input, input_len, (void *)output, output_len);

    show_arr_double(input, input_len);
    show_arr_double(output, output_len);

    free(p);
    free(input);
    free(output);
    return 0;
}

int unit_int16(uint32_t input_rate, uint32_t input_len,
               uint32_t output_rate, uint32_t output_len) {
    assert(output_rate * input_len == input_rate * output_len);
    size_t sz = src_memReq();
    void *p = malloc(sz);

    src_set(p, input_rate, output_rate, sizeof(int16_t) * 2, sizeof(int16_t));

    int16_t *input = malloc(sizeof(int16_t) * 2 * input_len);
    int16_t *output = malloc(sizeof(int16_t) * 2 * output_len);
    uint32_t i;
    for (i = 0; i != input_len; i++) {
        double f = sin(i / 20.);
        int16_t t = (sin(i / 20.) * 0x8000);
        input[2 * i] = input[2 * i + 1] = t;
    }
    show_arr_int16(input, input_len);

    src_conv_int16(p, (void *)input, input_len, (void *)output, output_len);

    show_arr_int16(output, output_len);

    free(p);
    free(input);
    free(output);
    return 0;
}

int unit_main() {
    unit_int16(3, 120, 2, 80);
    unit_int16(2, 80, 3, 120);
    unit_int16(11, 110, 12, 120);
    unit_int16(147, 147, 160, 160);
    return 0;
}

int main(int argc, char **argv) {
    const int in_rate = 147;
    const int out_rate = 160;
    if (argc != 2) {
        printf("%s [16bit_pcm_file]\n", argv[0]);
        return -1;
    }
    FILE *fp = fopen(argv[1], "rb");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    size = (size / in_rate) * in_rate;
    void *in = malloc(size);
    fread(in, size, 1, fp);
    fclose(fp);
    printf("load file=%s to buf=%p size=%lu\n", argv[1], in, size);

    long out_size = size / in_rate * out_rate;
    void *out = malloc(out_size);
    printf("preprate out=%p size=%lu\n", out, out_size);

    size_t sz = src_memReq();
    void *ctx = malloc(sz);
    printf("src conv %d to %d\n", in_rate, out_rate);
    size_t elemSize = sizeof(int16_t) * 2;
    src_set(ctx, in_rate, out_rate, elemSize, elemSize);
    printf("src conv in=%p size=%lu out=%p size=%lu\n",
           in, size, out, out_size);
    src_conv_int16(ctx, in, size / elemSize, out, out_size / elemSize);
    free(ctx);
    free(in);

    FILE *fp1 = fopen("out.raw", "wb");
    fwrite(out, out_size, 1, fp1);
    fclose(fp1);

    free(out);

    return 0;
}
