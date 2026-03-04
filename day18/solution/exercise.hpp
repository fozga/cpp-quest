#pragma once

#include <vector>
#include <ranges>
#include <cstddef>

bool is_prime(std::size_t n);

template <std::ranges::input_range R>
auto prime_filter(R&& rng) {
    return std::forward<R>(rng)
         | std::views::filter([](auto v) {
               return is_prime(static_cast<std::size_t>(v));
           });
}

std::vector<std::size_t> first_n_primes(std::size_t n);
