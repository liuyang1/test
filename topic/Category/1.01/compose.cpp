#include <functional>
#include <stdio.h>
#include <iostream>
#include <iomanip>

// It's freaky to write a function which input two function and reurn one function, this function
// type is input a int, and return a int.
// :(
// int (*compose(int (*f)(int), int (*g)(int)))(int) {
std::function<int (int)> compose(int (*f)(int), int (*g)(int)) {
    /**
     * [=] capture by value
     * [&] capture by reference
     */
    return [=](int x) { return f(g(x)); };
}

int inc(int x) {
    return x + 1;
}

int square(int x) {
    return x * x;
}

int id(int x) {
    return x;
}

#define TESTCASE(f, g) \
    std::cout << "(" #f " . " #g ")(" << a << ") = " << (compose(f, g))(a) << std::endl;

int main() {
    int a = 5;
    TESTCASE(inc, square);
    TESTCASE(square, inc);

    TESTCASE(inc, id);
    TESTCASE(id, inc);

    TESTCASE(square, id);
    TESTCASE(id, square);
    return 0;
}
