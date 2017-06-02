#include <stdio.h>

int func(const char *s) {
    return 10;
}

int main() {
    func("Run !!");
    static const char warning[] __attribute__((section(".gnu.warning.printf"))) =
        "My sweet linker warning";
    return 0;
}
