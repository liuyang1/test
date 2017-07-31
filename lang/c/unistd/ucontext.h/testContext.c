#include <stdio.h>
#include <ucontext.h>

void func() {
    puts("In child routine\n");
    puts("return will auto resume back to un_link\n");
}

void context_test() {
    char stack[64 * 1024];

    ucontext_t child, main;
    getcontext(&child);
    child.uc_stack.ss_sp = stack;
    child.uc_stack.ss_size = sizeof(stack);
    // child.uc_stack.ss_flag = 0;
    child.uc_link = &main;

    makecontext(&child, func, 0);

    puts("save current routine to main, run child routine\n");
    swapcontext(&main, &child);
    puts("Back to main routine\n");
}

int main() {
    context_test();
    return 0;
}
