/*
 * POJ 1001 Exponentiation
 * Big Integer Arithmetic
 * Using string for fast implement
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define LOG(...) fprintf(stderr, __VA_ARGS__)

// helper function
char toInt(char c) {
    return c - '0';
}

char toChar(char c) {
    return c + '0';
}

#define LEN 601
typedef struct {
    char decs[LEN]; // data storage at decs from LOW to HIGH
    int dotpos; // dot position
    int len; // actually fill length in decs
} bigint;

/*
 * API
 * big_init                 init bigint with char string
 * big_free
 * big_copy                 copy from other bigint
 * big_dup                  duplicate bigint, must call big_free after use
 * big_show                 print bigint to stdout
 *
 * big_add(a, b)            a += b
 * big_mul(a, b)            a *= b
 * big_pow_scalar(a, b)     a **= b
 * big_mul_scalar(a, b)     multiple
 *
 * Internal function:
 * big_pad                  add padding zero at low part of bigint
 * big_unpad                remove padding zero at low part of bigint
 * big_strip                remove all padding zero at low part of bigint
 * big_sync                 make sure two bigint have same dotpos
 * big_ext                  add padding zero at high part of bigint
 *
 * Debug function:
 * big_log                  print detail info of bigint
 */
bigint *big_init(char *s) {
    int i, j, dotpos = 0;
    bigint *r = malloc(sizeof(bigint));
    for (i = strlen(s) - 1, j = 0; i >= 0; i--) {
        if (s[i] == '.') {
            dotpos = j;
            continue;
        }
        r->decs[j] = s[i];
        j++;
    }
    r->decs[j] = '\0';
    r->dotpos = dotpos;
    r->len = j;
    return r;
}

void big_free(bigint *a) {
    free(a);
}

void big_copy(bigint *b, bigint *a) {
    strcpy(b->decs, a->decs);
    b->len = a->len;
    b->dotpos = a->dotpos;
}

bigint *big_dup(bigint *a) {
    bigint *b = malloc(sizeof(bigint));
    big_copy(b, a);
    return b;
}

void big_log(bigint *a) {
    LOG("%s dot=%d len=%d\n", a->decs, a->dotpos, a->len);
}

void big_pad(bigint *a, int pad) {
    if (pad < 0) {
        LOG("pad=%d < 0\n", pad);
    }
    if (pad == 0) {
        return;
    }
    int i;
    for (i = a->len + 1; i >= 0; i--) {
        a->decs[i + pad] = a->decs[i];
    }
    for (i = pad - 1; i >= 0; i--) {
        a->decs[i] = '0';
    }
    a->dotpos += pad;
    a->len += pad;
}

void big_unpad(bigint *a, int pad) {
    if (pad == 0) {
        return;
    }
    int i;
    for (i = pad; i <= a->len + 1; i++) {
        a->decs[i - pad] = a->decs[i];
    }
    a->dotpos -= pad;
    a->len -= pad;
}


void big_strip(bigint *a) {
    int i;
    for (i = 0; i != a->dotpos; i++) {
        if (a->decs[i] != '0') {
            break;
        }
    }
    big_unpad(a, i);
}

void big_sync(bigint *a, bigint *b) {
    int dotpos = MAX(a->dotpos, b->dotpos);
    big_pad(a, dotpos - a->dotpos);
    big_pad(b, dotpos - b->dotpos);
}

void big_show(bigint *a) {
    big_strip(a);
    int i, lead = 1;
    for (i = a->len - 1; i >= 0; i--) {
        if (lead && a->decs[i] == '0' && i > a->dotpos - 1) {
            continue;
        }
        lead = 0;
        if (i == a->dotpos - 1) {
            putchar('.');
        }
        putchar(a->decs[i]);
    }
    putchar('\n');
}


void big_ext(bigint *a, int len) {
    int i;
    for (i = a->len; i != len; i++) {
        a->decs[i] = '0';
    }
    a->decs[i] = '\0';
    a->len = len;
}

void big_add(bigint *a, bigint *b) {
    big_sync(a, b);
    int i, sum, carry;
    int len = MAX(a->len, b->len) + 1;
    big_ext(a, len);
    big_ext(b, len);
    for (i = 0, carry = 0; i != len; i++) {
        sum = toInt(a->decs[i]) + toInt(b->decs[i]) + carry;
        a->decs[i] = toChar(sum % 10);
        carry = sum / 10;
    }
}

void big_mul_scalar(bigint *a, int b) {
    if (b <= 0) {
        LOG("not support b=%d <= 0\n", b);
        return;
    } else if (b == 1) {
        return;
    }
    int i, mul, carry;
    int len = a->len + 1;
    big_ext(a, len);
    for (i = 0, carry = 0; i != len; i++) {
        mul = toInt(a->decs[i]) * b + carry;
        a->decs[i] = toChar(mul % 10);
        carry = mul / 10;
    }
}

void big_mul(bigint *a, bigint *b) {
    bigint *acc = big_init("0"), *t;
    t = big_dup(a);
    for (int i = 0; i != b->len; i++) {
        if (toInt(b->decs[i]) == 0) {
            continue;
        }
        big_copy(t, a);
        big_mul_scalar(t, toInt(b->decs[i]));
        t->dotpos += (b->dotpos - i);
        big_add(acc, t);
    }
    big_copy(a, acc);
    big_free(acc);
    big_free(t);
    return;
}

void big_pow_scalar(bigint *a, int p) {
    bigint *b;
    if (p <= 0) {
        LOG("not support p=%d <= 0\n", p);
    } else if (p == 1) {
    } else if (p == 2) {
        b = big_dup(a);
        big_mul(a, b);
        big_free(b);
    } else if (p % 2 == 0) {
        big_pow_scalar(a, p / 2);
        big_pow_scalar(a, 2);
    } else {
        b = big_dup(a);
        big_pow_scalar(a, p / 2);
        big_pow_scalar(a, 2);
        big_mul(a, b);
        big_free(b);
    }
}

void big_test(bigint *a) {
    big_pad(a, 2);
    big_log(a);

    bigint *b = big_dup(a);
    big_add(a, b);
    big_log(a);
    big_free(b);

    big_mul_scalar(a, 2);
    big_log(a);

    bigint *t = big_init("2.1");
    big_mul(a, t);
    big_log(a);
    big_free(t);

    big_pow_scalar(a, 3);
    big_log(a);
}

int main() {
    char s[1024];
    int b;
    while (scanf("%s%d", s, &b) != EOF) {
        bigint *a = big_init(s);
        big_pow_scalar(a, b);
        big_show(a);
        big_free(a);
    }
    return 0;
}
