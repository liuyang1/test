#include "../kr.h"

#define errmsg(...) fprintf(stderr, "error: "__VA_ARGS__)
#define MAXOP       100
typedef enum {
    eEOF = EOF,
    eNumber = '0',
    eOperator = 'o',
    eVarName = 'v',
    eAdd = '+',
    eMinus = '-',
    eMul = '*',
    eDiv = '/',
    eMod = '%',
    eEnd = '\n',
} OpType;

#define BUFSIZE 100
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
// getop: get next operator or numeric operand
OpType getop(char *s) {
    int i, c;
    while ((s[0] = c = getch()) == ' ' || c == '\t') {
        ;
    }
    s[1] = '\0';
    if (!isdigit(c) && c != '.') {
        return c;
    }
    i = 0;
    if (isdigit(c)) {
        // collect integer part
        while (isdigit(s[++i] = c = getch())) {
            ;
        }
    }
    if (c == '.') {
        while (isdigit(s[++i] = c = getch())) {
            ;
        }
    }
    s[i] = '\0';
    if (c != EOF) {
        ungetch(c);
    }
    return eNumber;
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
        return;
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

int rpn() {
    OpType type;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != eEOF) {
        switch (type) {
            case eNumber:
                push(atof(s));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() - pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':
                op2 = pop();
                if (op2 == 0.0) {
                    errmsg("zero divisor\n");
                    return -1;
                }
                push(pop() / op2);
                break;
            case '%':
                op2 = pop();
                if (op2 == 0.0) {
                    errmsg("zero modulor\n");
                    return -1;
                }
                push((int)(pop()) % (int)op2);
                break;
            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                errmsg("unknown command %s\n", s);
                break;
        }
    }
    return 0;
}

int main() {
    rpn();
    return 0;
}
