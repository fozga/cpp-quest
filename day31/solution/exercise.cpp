#include "exercise.hpp"
#include <ostream>
#include <numeric>
#include <algorithm>
#include <limits>

Stats compute_stats(const std::vector<int>& values) {
    Stats s;
    s.count_total = values.size();

    if (values.empty()) {
        return s;
    }

    s.min = *std::min_element(values.begin(), values.end());
    s.max = *std::max_element(values.begin(), values.end());

    long long sum = std::accumulate(values.begin(), values.end(), 0LL);
    s.average = static_cast<double>(sum) / static_cast<double>(s.count_total);

    for (int v : values) {
        if (v > 0)      ++s.count_positive;
        else if (v < 0) ++s.count_negative;
        else             ++s.count_zero;
    }

    return s;
}

void print_stats(const Stats& s, std::ostream& os) {
    os << "count: " << s.count_total  << '\n'
       << "min: "   << s.min          << '\n'
       << "max: "   << s.max          << '\n'
       << "avg: "   << s.average      << '\n'
       << "pos: "   << s.count_positive << '\n'
       << "neg: "   << s.count_negative << '\n'
       << "zero: "  << s.count_zero   << '\n';
}

void legacy_print_stats(const std::vector<int>& values, std::ostream& os) {
    print_stats(compute_stats(values), os);
}
