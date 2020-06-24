/** extract audiod data as channels  32bit only*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
void usage(char *name) {
    printf("%s file chn-all chn-begin chn-number\n", name);
}

int main(int argc, char **argv) {
    char *name = argv[0];
    if (argc < 5) {
        usage(name);
        return -1;
    }
    char *fn = argv[1];
    int chn = atoi(argv[2]);
    int bgn = atoi(argv[3]);
    int num = atoi(argv[4]);
    if (chn == 0 || num == 0) {
        usage(name);
        return -1;
    }

    FILE *in = fopen(fn, "ro");
    char out_fn[128] = {0};
    sprintf(out_fn, "%s.%d.%d.%d", fn, chn, bgn, num);
    FILE *out = fopen(out_fn, "w");
    int32_t i;
    for (i = 0; ; i++) {
        int x;
        int ret = fread(&x, sizeof(int32_t), 1, in);
        if (ret != 1) {
            break;
        }
        int r = i % chn;
        if (bgn <= r && r < bgn + num) {
            fwrite(&x, sizeof(int32_t), 1, out);
        }
    }
    fclose(in);
    fclose(out);
    return 0;
}
