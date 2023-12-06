#include "alog.h"

#define ERROR(...) ALOG(ADECODER, LERROR, __VA_ARGS__)
int main() {
    ERROR("hello");
    return 0;
}
