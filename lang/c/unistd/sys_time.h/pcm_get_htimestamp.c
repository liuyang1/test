#include "asound.h"

int main() {
    struct pcm_config cfg;
    memset(&cfg, 0, sizeof(cfg));
    void *p = pcm_open(0, 0, 0, &cfg);
    printf("%p\n", p);
    pcm_close(p);
    return 0;
}
