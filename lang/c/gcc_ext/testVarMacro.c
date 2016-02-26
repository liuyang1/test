/*
Ref: [[https://gcc.gnu.org/onlinedocs/gcc-3.0.1/cpp_3.html]]
 ** 3.6 Variadic Macros

 If your macro is complicated, you may want a more descriptive name for the
 variable argument than __VA_ARGS__. GNU CPP permits this, as an extension.
 You may write an argument name immediately before the `...'; that name is used
 for the variable argument. The eprintf macro above could be written

    #define eprintf(args...) fprintf (stderr, args)

 using this extension. You cannot use __VA_ARGS__ and this extension in the
 same macro.
*/

#include <stdio.h>
#define eprintf(args...) fprintf(stderr, args)
#define dprintf(...)     fprintf(stderr, __VA_ARGS__)
#define efprintf(fmt, args...) fprintf(stderr, "efprintf " fmt, args)

int main() {
    eprintf("test Variadic Macro ext by gcc\n");
    int i = 100;
    eprintf("i=%d\n", i);

    dprintf("test Variadic Macro std C\n");
    dprintf("i=%d\n", i);

    // compiler error, as NO param to match args
    // efprintf("test Variadic Macro with format\n");
    efprintf("%s", "test Variadic Macro with format\n");
    efprintf("i=%d\n", i);
    return 0;
}
