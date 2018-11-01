#include <stdio.h>
#include <strings.h>

int main() {
    FILE *fp = fopen("testfile", "r");
    char buf[128];
    while (!feof(fp)) {
        bzero(buf, 128);
        size_t n = fread(buf, 1, 6, fp);
        printf("n=%lu read buf=%s\n", n, buf);
        // feof will check 3times, even the file's content only 6bytes (including newline)
        // #1 seek point 0, feof is false, fread return "Hello"
        // #2 seek point 6, feof is false, fread fail, n => 0
        // #3 seek point 6, feof is TRUE, stop loop
    }
    fclose(fp);
    return 0;
}
