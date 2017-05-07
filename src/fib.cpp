#include <algorithm>
#include "fib.h"

int
fib(int n) {
    if (n <= 0) {
        return 0;
    }
    int a = 0, b = 1;
    while (n-- > 0) {
        std::swap(a, b);
        b += a;
    }
    return a;
}
