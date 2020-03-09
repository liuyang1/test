#include <stdio.h>
extern const char my_data[];
// extern char my_data_end;
extern const int my_data_len;
// int my_data_len = sizeof(my_data) / sizeof(char);

int main() {
    printf("%p\n", my_data);
    printf("%02x%02x%02x%02x\n", my_data[0], my_data[1],
           my_data[2], my_data[3]);
    // printf("%p\n", &my_data_end);
    // printf("%d\n", &my_data_end - my_data);
    printf("%p\n", &my_data_len);
    printf("%d\n", my_data_len);
    return 0;
}
