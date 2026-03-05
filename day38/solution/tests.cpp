#include "exercise.hpp"
#include <cassert>
#include <iostream>

int main() {
    assert(sum({1, 2, 3}) == 6);
    assert(sum({}) == 0);
    assert(sum({-1, 1}) == 0);
    std::cout << "[PASS] sum" << std::endl;

    assert(join({1, 2, 3}, ',') == "1,2,3");
    assert(join({42}, '-') == "42");
    assert(join({}, ',') == "");
    std::cout << "[PASS] join" << std::endl;

    assert(run_demo() == "sum=6; values=1,2,3");
    std::cout << "[PASS] run_demo" << std::endl;

    std::cout << "All day-38 tests passed." << std::endl;
    return 0;
}
