#include "exercise.hpp"
#include <cmath>

bool is_prime(std::size_t n) {
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (std::size_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

std::vector<std::size_t> first_n_primes(std::size_t n) {
    auto view = prime_filter(std::views::iota(std::size_t{2}))
              | std::views::take(static_cast<std::ptrdiff_t>(n));
    std::vector<std::size_t> result;
    for (auto v : view) result.push_back(v);
    return result;
}
