#include "exercise.hpp"

#include <cassert>
#include <iostream>

int main() {
    std::cout << "=== Day 13: Type-Safe Unions ===\n\n";

    auto lit = make_int(42);
    assert(evaluate(lit) == 42);
    std::cout << "  to_string(42)           = " << to_string(lit) << "\n";

    auto sum = make_add(make_int(1), make_int(2));
    assert(evaluate(sum) == 3);
    std::cout << "  to_string(1 + 2)        = " << to_string(sum) << "\n";

    auto prod = make_mul(make_int(2), make_add(make_int(3), make_int(4)));
    assert(evaluate(prod) == 14);
    std::cout << "  to_string(2 * (3 + 4))  = " << to_string(prod) << "\n";

    auto complex = make_add(make_mul(make_int(2), make_int(3)), make_int(1));
    assert(evaluate(complex) == 7);
    std::cout << "  to_string((2*3) + 1)    = " << to_string(complex) << "\n";

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
