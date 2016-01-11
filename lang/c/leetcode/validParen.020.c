#include "leet.h"
typedef struct {
    char *a;
    int dep;
    int len;
} Stack;

#define STEP 4
void initStack(Stack *p) {
    p->len = STEP;
    p->a = malloc(sizeof(char) * p->len);
    p->dep = 0;
}
void finitStack(Stack *p) {
    free(p->a);
}
void push(Stack *p, char c) {
    p->a[p->dep] = c;
    p->dep++;
    if (p->dep >= p->len) {
        p->len += STEP;
        LOG("increase step to %d\n", p->len);
        reallocM(&(p->a), sizeof(char) * p->len);
    }
}
bool pop(Stack *p, char *pc) {
    if (p->dep == 0) {
        return false;
    }
    p->dep--;
    *pc = p->a[p->dep];
    return true;
}
bool isValid(char* s) {
    Stack stk, *p = &stk;
    initStack(p);
    char c;
    bool r;
    for (; *s != '\0'; s++) {
        if (*s == '(' || *s == '{' || *s == '[') {
            push(p, *s);
        } else if (*s == ')' || *s == '}' || *s == ']') {
            r = pop(p, &c);
            if (r == false) {
                return false;
            }
            if (!((*s == ')' && c == '(') ||
                  (*s == '}' && c == '{') ||
                  (*s == ']' && c == '['))) {
                return false;
            }
        } else {
            return false;
        }
    }
    r = pop(p, &c);
    finitStack(p);
    return r == false;
}

#define CASE(s, b) {bool r = isValid(s); \
    printf("%s %s ?= %s %s\n", \
           s, SBOOL(r), SBOOL(b), expect(r == b)); }
int main() {
    CASE("[", false);
    CASE("[]{}()", true);
    CASE("[{()}()]", true);
    CASE("[{()()]", false);
    CASE("[{([{()}])}]", true);
    CASE("a", false);
    return 0;
}
