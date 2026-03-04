#include "exercise.hpp"
#include <cassert>
#include <iostream>

int main() {
    assert(!is_prime(0));
    assert(!is_prime(1));
    assert(is_prime(2));
    assert(is_prime(3));
    assert(!is_prime(4));
    assert(is_prime(5));
    assert(!is_prime(9));
    assert(is_prime(11));
    std::cout << "is_prime tests passed\n";

    auto primes5 = first_n_primes(5);
    assert(primes5.size() == 5);
    assert(primes5[0] == 2 && primes5[1] == 3 && primes5[2] == 5 && primes5[3] == 7 && primes5[4] == 11);
    std::cout << "first_n_primes(5) passed\n";

    auto primes10 = first_n_primes(10);
    assert(primes10.size() == 10);
    assert(primes10[9] == 29);
    std::cout << "first_n_primes(10) passed\n";

    std::cout << "All day 18 tests passed!\n";
    return 0;
}
