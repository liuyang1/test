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

int main() {
    int a = 5;
    std::cout << "(inc . square)(" << a << ") = " << (compose(inc, square))(a) << std::endl;
    std::cout << "(square . inc)(" << a << ") = " << (compose(square, inc))(a) << std::endl;

    std::cout << "(inc . id)(" << a << ") = " << (compose(inc, id))(a) << std::endl;
    std::cout << "(id . inc)(" << a << ") = " << (compose(id, inc))(a) << std::endl;

    std::cout << "(id . square)(" << a << ") = " << (compose(id, square))(a) << std::endl;
    std::cout << "(square . id)(" << a << ") = " << (compose(square, id))(a) << std::endl;
    return 0;
}
