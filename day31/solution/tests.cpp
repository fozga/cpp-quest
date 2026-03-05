#include "exercise.hpp"
#include <cassert>
#include <iostream>
#include <sstream>
#include <cmath>

int main() {
    // Test 1: empty vector
    {
        std::vector<int> empty;
        Stats s = compute_stats(empty);
        assert(s.count_total == 0);
        std::cout << "  [pass] empty vector\n";
    }

    // Test 2: mixed values {1,2,3,-1,0}
    {
        std::vector<int> v{1, 2, 3, -1, 0};
        Stats s = compute_stats(v);
        assert(s.count_total == 5);
        assert(s.min == -1);
        assert(s.max == 3);
        assert(s.count_positive == 3);
        assert(s.count_negative == 1);
        assert(s.count_zero == 1);
        assert(std::fabs(s.average - 1.0) < 1e-9);
        std::cout << "  [pass] mixed values {1,2,3,-1,0}\n";
    }

    // Test 3: uniform values {5,5,5}
    {
        std::vector<int> v{5, 5, 5};
        Stats s = compute_stats(v);
        assert(s.min == 5);
        assert(s.max == 5);
        assert(std::fabs(s.average - 5.0) < 1e-9);
        assert(s.count_positive == 3);
        std::cout << "  [pass] uniform values {5,5,5}\n";
    }

    // Test 4: legacy_print_stats output format
    {
        std::vector<int> v{1, 2, 3, -1, 0};
        std::ostringstream oss;
        legacy_print_stats(v, oss);
        std::string output = oss.str();
        assert(output.find("count:") != std::string::npos);
        assert(output.find("min:") != std::string::npos);
        assert(output.find("max:") != std::string::npos);
        std::cout << "  [pass] legacy_print_stats output format\n";
    }

    std::cout << "All tests passed.\n";
    return 0;
}
