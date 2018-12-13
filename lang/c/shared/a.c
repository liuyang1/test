// int func_nest(int a) {
//     return a * 5;
// }
//
#include "third.h"
int func_a(int a) {
    // return func_nest(a) * 2;
    return a * 2 * func_third(a);
}
