/** helper code to convert 'xxd -i' file back to binary
 */
#include <stdio.h>
#include "speex-wav.c"

int main() {
    void *p = sample_6s_speex_wav;
    size_t size = sample_6s_speex_wav_len;
    fwrite(p, 1, size, stdout);
    return 0;
}
