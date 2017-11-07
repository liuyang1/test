#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    char *arr;
    int len;
    int idx;
} stack;

void create(stack *p, int len) {
    p->arr = malloc(sizeof(char) * len);
    p->len = len;
    p->idx = 0;
}

void destory(stack *p) {
    free(p->arr);
}

void push(stack *p, char c) {
    p->arr[p->idx++] = c;
    assert(p->idx < p->len);
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
    stack stk, *p = &stk;
    create(p, 4096); // known issue: limit stack size
    for (; *s != '\0'; s++) {
        char c = peek(&stk);
        if ((*s == '}' && c == '{') ||
            (*s == ']' && c == '[') ||
            (*s == ')' && c == '(')) {
            pop(p);
        } else {
            push(p, *s);
        }
    }
    destory(p);
    return empty(p);
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
    return 0;
}

int main() {
    test();
    return 0;
}
