#include "exercise.hpp"
#include <cassert>
#include <iostream>

int main() {
    {
        std::vector<int> v{1, 2, 3, 4, 5};
        auto fut = async_accumulate(v);
        int result = fut.get();
        assert(result == 15);
        std::cout << "[PASS] async_accumulate({1..5}) == 15" << std::endl;
    }
    {
        std::vector<int> v{};
        auto fut = async_accumulate(v);
        assert(fut.get() == 0);
        std::cout << "[PASS] async_accumulate({}) == 0" << std::endl;
    }
    {
        std::vector<int> v{100, 200, 300};
        auto fut = async_accumulate_ll(v);
        assert(fut.get() == 600LL);
        std::cout << "[PASS] async_accumulate_ll({100,200,300}) == 600" << std::endl;
    }
    std::cout << "All day-25 tests passed." << std::endl;
    return 0;
}
