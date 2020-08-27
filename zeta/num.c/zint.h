/** simple big data with char string
 * support add, multiply, minus, divide, modulo
 */

#ifndef _ZINT_H_
#define _ZINT_H_

#include <stdint.h> // for uint8_t
#include <stdlib.h> // for size_t

typedef uint8_t elem_t;
#define BASE        256
#define BASE_MAX    UCHAR_MAX

typedef struct {
    int sign;
    size_t n; // length of valid sequence
    elem_t *s; // sequence, should not leading with zero
} zint_t;
/** Special case:
 * For 0, sign=0, n = 0, s = NULL
 * For positive number, sign=1, n=N, s=valid pointer
 * For negative number, sign=-1, n=N, s=valid pointer
 */
extern zint_t ZINT_ZERO;

void zint_free(zint_t x);

void zint_show(zint_t x);
void zint_debug(zint_t x);
size_t zint_print(char *s, size_t n, zint_t x);

// zint_t zint_ctor_int(int x);
 // strings ends with '\0'
zint_t zint_ctor_str(char *s);
zint_t zint_copy(zint_t x, size_t n);

int zint_sgn(zint_t a);
int zint_cmp(zint_t a, zint_t b);
zint_t zint_neg(zint_t x);
zint_t zint_add(zint_t a, zint_t b);
zint_t zint_minus(zint_t a, zint_t b);
zint_t zint_mul(zint_t a, zint_t b);

#endif
