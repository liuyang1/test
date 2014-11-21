/* copyright @ liuyang1@mail.ustc.edu.cn
 * 2014-08-14 22:28:22
 *
 * product algo
 * as generator and yield
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LEN             3

typedef unsigned int index_t;
typedef char (string_t)[1024];

// key struct
typedef struct Prod_t {
    string_t *pointer;      // pointer of string array
    index_t *array;         // counter array
    index_t *carry;         // carry array
    index_t n;
} Prod_t;

typedef enum Inc_flag_t {
    CONTINUE = 0,
    END,
} Inc_flag_t;

void init_Prod_t(Prod_t *p, string_t *pointer, int n)
{
    p->pointer = pointer;
    p->n = n;
    p->array = (index_t *)malloc(sizeof(index_t) * n);
    p->carry = (index_t *)malloc(sizeof(index_t) * n);
    index_t i;
    for (i = 0; i < n; i++, pointer++) {
        p->array[i] = 0;
        // init carry array using strlen of each string
        p->carry[i] = strlen((const char*)pointer);
    }
}

void release_Prod_t(struct Prod_t *p)
{
    free(p->array);
    free(p->carry);
    bzero(p, sizeof(*p)); // clear data for safty
    p = NULL;
}

Inc_flag_t increment(struct Prod_t *p)
{
    index_t i;
    index_t *arr;
    index_t *carry;

    for (i = 0, arr = p->array, carry = p->carry;
            i < p->n; i++, arr++, carry++) {
        // increment
        *arr = *arr + 1;
        // check if need to carry
        if (*arr != *carry) {
            break;
        }
        // if carry to highest level, then END
        if (i == p->n - 1) {
            return END;
        }
        // when carry to next level, this level reset to zero
        *arr = 0;
    }
    return CONTINUE;
}

void dump_str(struct Prod_t *p, char *str)
{
    index_t i;
    for (i = 0; i < p->n; i++, str++) {
        // dump one combination
        *str = p->pointer[i][p->array[i]];
    }
    *str = '\0';
}

int main()
{
    string_t strarr[LEN] = {"012", "ab", "xyz"};
    string_t str;
    struct Prod_t prod;

    init_Prod_t(&prod, strarr, LEN);

    // keep to dump_str, keep to continue
    do {
        dump_str(&prod, str);
        printf("%s\n", str);
    } while (increment(&prod) == CONTINUE);

    release_Prod_t(&prod);
    return 0;
}
