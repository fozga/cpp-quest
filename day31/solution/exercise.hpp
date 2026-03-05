#pragma once
#include <vector>
#include <cstddef>
#include <iosfwd>

struct Stats {
    int min = 0;
    int max = 0;
    double average = 0.0;
    std::size_t count_positive = 0;
    std::size_t count_negative = 0;
    std::size_t count_zero = 0;
    std::size_t count_total = 0;
};

Stats compute_stats(const std::vector<int>& values);
void print_stats(const Stats& s, std::ostream& os);
void legacy_print_stats(const std::vector<int>& values, std::ostream& os);
