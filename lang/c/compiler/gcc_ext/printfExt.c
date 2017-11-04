// ref:
// - http://codingrelic.geekhold.com/2008/12/printf-acular.html
// - https://www.gnu.org/software/libc/manual/html_node/Printf-Extension-Example.html


#include <stdio.h>
#include <printf.h>

/**
 * struct printf_info {
 *      int prec; // precision
 *      int width;
 *      wchar_t spec;
 *      unsigned int is_long_double;    // if have L, ll, q modifier
 *      unsigned int is_char;           // if have hh modifier
 *      unsigned int is_short;          // if have h modifier
 *      unsigned int is_long;           // if have l modifier
 *      unsigned int alt;               // if have # flag
 *      unsigned int space;             // if have ' ' flag
 *      unsigned int left;              // if have '-' flag
 *      unsigned int showsign;          // if have '+' flag
 *      unsigned int group;             // if have '`` flag
 *      unsigned int extra;             // user-defined
 *      unsigned int wide;              // the stream is wide or not
 *      wchar_t pad;                    // padding char, '0' or ' '
 * }
 */
static int printf_arginfo_M(const struct printf_info *info, size_t n, int *argtypes) {
    if (n > 0) {
        argtypes[0] = PA_POINTER; // mac address only have one argument
    }
    return 1;
}

static int printf_output_M(FILE *stream, const struct printf_info *info, const void *const *args) {
    const unsigned char *mac;
    int len;
    mac = *(unsigned char **)(args[0]);
    len = fprintf(stream, "%02x:%02x:%02x:%02x:%02x:%02x",
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return len;
}

int try_printf_mac() {
    unsigned char mac[] = {0, 1, 2, 3, 4, 5};
    // register_printf_function function is deprecated now.
    if (register_printf_function('M', printf_output_M, printf_arginfo_M)) {
        printf("cannot register %M in printf\n");
        return 1;
    }
    printf("%30M\n", mac);
    return 0;
}

int main() {
    try_printf_mac();
    return 0;
}
