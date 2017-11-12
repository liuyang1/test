/**
 * CHANGES: 2017-11-12 improve stack implement;
 *                     support unlimited stack depth
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    char *arr;
    int len;
    int idx;
} stack;

/** stack whose element is char
 */
void *create(int len) {
    stack *p = malloc(sizeof(stack));
    p->arr = malloc(sizeof(char) * len);
    p->len = len;
    p->idx = 0;
    return p;
}

void destory(stack *p) {
    free(p->arr);
    free(p);
}

void chkExtend(stack *p) {
    if (p->idx == p->len) {
        p->len *= 2;
        void *np = realloc(p->arr, sizeof(char) * p->len);
        if (np == NULL) {
            perror("Out Of Memory\n");
            abort();
        }
        p->arr = np;
    }
}

void push(stack *p, char c) {
    chkExtend(p);
    assert(p->idx < p->len);
    p->arr[p->idx++] = c;
}

char pop(stack *p) {
    return p->arr[--p->idx];
}

char peek(stack *p) {
    return p->idx <= 0 ? '\0' : p->arr[p->idx - 1];
}

bool empty(stack *p) {
    return p->idx == 0;
}

/* Inspired by Haskell solution
validBraces_ = null . foldr op []
    where op '{' ('}': xs) = xs
          op '[' (']': xs) = xs
          op '(' (')': xs) = xs
          op x xs = x: xs
*/
bool valid_braces(char *s) {
    stack *p = create(2);
    for (; *s != '\0'; s++) {
        char c = peek(p);
        if ((*s == '}' && c == '{') ||
            (*s == ']' && c == '[') ||
            (*s == ')' && c == '(')) {
            pop(p);
        } else {
            push(p, *s);
        }
    }
    bool ret = empty(p);
    destory(p);
    return ret;
}

// test code
#include "test.h"
bool unit(char *s, bool expect) {
    bool ret = valid_braces(s);
    bool r = ret == expect;
    if (!r) {
        printf("valid_braces(%s) = %s !!!= %s\n",
               s, showBool(ret), showBool(expect));
    }
    return r;
}

int test() {
    unit("(){}[]", true);
    unit("([{}])", true);
    unit("(}", false);
    unit("[(])", false);
    unit("[({})](])])", false);
    unit("()", true);
    unit("[([)", false);
    unit("())({}}{()][][", false);
    unit("({})[({})]", true);
    unit("((((((((((((((((((((((()))))))))))))))))))))))", true);
    return 0;
}

int main() {
    test();
    return 0;
}
