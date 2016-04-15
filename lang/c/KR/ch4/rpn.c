#include "../kr.h"

#define debug
#ifdef debug
#define log(fmt, ...)   printf("%s:%d " fmt "\n", __FUNCTION__, __LINE__, \
                               __VA_ARGS__);
#else
#define log(...)
#endif
#define errmsg(...)     fprintf(stderr, "error: "__VA_ARGS__)
#define MAXOP           100
typedef enum {
    eNumber = '0',
    eOperator = 'o',
    // eVarName = 'v',
} OpType;

#define BUFSIZE         100
char buf[BUFSIZE];
int bufp = 0;
int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp >= BUFSIZE) {
        errmsg("too many char\n");
        return;
    }
    buf[bufp++] = c;
}

int untilNotSpace() {
    int c;
    while (isspace(c = getch())) {
        ;
    }
    return c;
}

void skipline() {
    int c;
    while ((c = getch()) != '\n' && c != EOF) {
        printf("%c", c);
    }
    ;
    printf("\n");
}

bool oneOf(char c, char s[]) {
    for (; *s != '\0'; s++) {
        if (c == *s) {
            return true;
        }
    }
    return false;
}

bool isSignChr(char c) {
    return oneOf(c, "+-");
}

/**
 * @brief get next integer from input into *pn
 *
 * @param OUT pn
 *
 * @return if have integer, return integer with pn, return last char
 * if not have integer, pn = 0, return char zero
 */
int getint(int *pn) {
    *pn = 0; // init it to zero
    int pc = -1, c, sgn = 1;
    c = untilNotSpace();
    if (isSignChr(c)) {
        pc = c;
        sgn = (c == '-') ? -1 : 1;
        c = getch();
    }
    if (!isdigit(c) && c != EOF) {
        ungetch(c);
        if (pc != -1) {
            ungetch(pc);
        }
        return 0;
    }
    for (*pn = 0; isdigit(c); c = getch()) {
        *pn = 10 * *pn + (c - '0');
    }
    *pn *= sgn;
    if (c != EOF) {
        ungetch(c);
    }
    return c;
}

int getfloat(double *pn) {
    *pn = 0;
    int c, v, sgn;
    double power = 1.0;
    c = getint(&v);
    *pn = v;
    if (c == 0) {
        return c;
    }
    sgn = (v < 0) ? -1 : 1;
    if ((c = getch()) == '.') {
        c = getch();
    }
    for (; isdigit(c); c = getch()) {
        *pn = 10 * *pn + sgn * (c - '0');
        power *= 10.0;
    }
    if (oneOf(c, "eE")) {
        int expPower = 0.0;
        c = getint(&expPower);
        if (c != 0) {
            power /= pow(10.0, expPower);
        } else {
            c = getch(); // peer with final ungetch
        }
    }
    *pn /= power;
    if (c != EOF) {
        ungetch(c);
    }
    return c;
}

bool isNumber(char s[]) {
    int i = 0;
    if (oneOf(s[i], "+-")) {
        i++;
    }
    if (isdigit(s[i]) || (s[i] == '.' && isdigit(s[i + 1]))) {
        return true;
    }
    return false;
}

// getop: get next operator or numeric operand
int getop(char *s) {
    int i, c;
    while ((s[0] = c = getch()) == ' ' || c == '\t') {
        ;
    }
    if (c == '\n' || c == EOF) {
        s[1] = '\0';
        return eOperator;
    }
    i = 0;
    while (!(oneOf(s[++i] = c = getch(), " \t\n"))) {
        ;
    }
    ungetch(c);
    s[i] = '\0';
    log("s=[%s]", s);
    return isNumber(s) ? eNumber : eOperator;
}

// maximum depth of val stack
#define MAXDEPTH    100
// next free stack position
int sp = 0;
// value stack
double val[MAXDEPTH];
// push: push f onto value stack
void push(double f) {
    if (sp >= MAXDEPTH) {
        errmsg("stack full, cannot push %g\n", f);
        exit(-1);
    }
    val[sp++] = f;
}

// pop: pop and return top value from stack
double pop(void) {
    if (sp <= 0) {
        errmsg("stack empty\n");
        return 0.0;
    }
    return val[--sp];
}

void clear() {
    while (sp != 0) {
        pop();
    }
}

int rpn() {
    OpType type;
    double op1, op2;
    char s[MAXOP];

    while (true) {
        type = getop(s);
        switch (type) {
            case eNumber:
                push(atof(s));
                break;
            case eOperator:
                if (strcmp(s, "+") == 0) {
                    push(pop() + pop());
                } else if (strcmp(s, "*") == 0) {
                    push(pop() * pop());
                } else if (strcmp(s, "-") == 0) {
                    op2 = pop();
                    push(pop() - op2);
                } else if (strcmp(s, "/") == 0) {
                    op2 = pop();
                    if (op2 == 0.0) {
                        errmsg("zero divisor\n");
                        return -1;
                    }
                    push(pop() / op2);
                } else if (strcmp(s, "%") == 0) {
                    op2 = pop();
                    if (op2 == 0.0) {
                        errmsg("zero modulor\n");
                        return -1;
                    }
                    push((int)(pop()) % (int)op2);
                } else if (strcmp(s, "\n") == 0) {
                    printf("\t%.8g\n", pop());
                } else if (strcmp(s, "print") == 0) {
                    op2 = pop();
                    printf("top\t%.8g\n", op2);
                    push(op2);
                } else if (strcmp(s, "dup") == 0) {
                    op2 = pop();
                    push(op2);
                    push(op2);
                } else if (strcmp(s, "swap") == 0) {
                    op1 = pop();
                    op2 = pop();
                    push(op1);
                    push(op2);
                } else if (strcmp(s, "clear") == 0) {
                    clear();
                } else if (strcmp(s, "sin") == 0) {
                    push(sin(pop()));
                } else if (strcmp(s, "exp") == 0) {
                    push(exp(pop()));
                } else if (strcmp(s, "pow") == 0) {
                    op2 = pop();
                    push(pow(pop(), op2));
                } else if (s[0] == EOF) {
                    log("%s", "end of file, exit");
                    return 0;
                } else {
                    errmsg("unknown command %x %s\n", s[0], s);
                }
                break;
            default:
                errmsg("unknown type=%x\n", type);
                break;
        }
    }
    return 0;
}

int unit_getint() {
    int v, c = getint(&v);
    printf("c=%d v=%d\n", c, v);
    if (c == 0) {
        printf("not number, skip this line\n");
        skipline();
    }
    return 0;
}

int unit_getfloat() {
    double v;
    int c = getfloat(&v);
    printf("c=%d v=%f\n", c, v);
    if (c == 0) {
        printf("not number, skip this line\n");
        skipline();
    }
    return 0;
}

int test_units(int (*unit_test)(void), int times) {
    int i;
    for (i = 0; i != times; i++) {
        unit_test();
    }
    return 0;
}

int test() {
    test_units(unit_getint, 4);
    test_units(unit_getfloat, 11);
    return 0;
}


int main() {
    rpn();
    return 0;
}
