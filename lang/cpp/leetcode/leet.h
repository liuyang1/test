#include "iostream"

template <typename T>
void showArr(const T *a, size_t len, const char* trail="") {
    printf("[");
    size_t i;
    for (i = 0; ; i++) {
        std::cout << a[i];
        if (i != len - 1) {
            printf(", ");
        } else {
            break;
        }
    }
    printf("]%s", trail);
}
