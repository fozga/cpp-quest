#include "exercise.hpp"

#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    RingBuffer<int> buf(5);

    assert(buf.empty());
    assert(buf.size() == 0);
    assert(buf.capacity() == 5);
    std::cout << "[pass] empty buffer\n";

    buf.push_back(1);
    buf.push_back(2);
    buf.push_back(3);
    assert(buf.size() == 3);
    assert(!buf.empty());
    std::cout << "[pass] push_back 1,2,3 — size == 3\n";

    std::vector<int> collected;
    for (auto& v : buf) {
        collected.push_back(v);
    }
    assert((collected == std::vector<int>{1, 2, 3}));
    std::cout << "[pass] range-based for yields {1,2,3}\n";

    buf.push_back(4);
    buf.push_back(5);
    buf.push_back(6);
    buf.push_back(7);
    assert(buf.size() == 5);
    std::cout << "[pass] after pushing 4,5,6,7 — size == 5 (full)\n";

    collected.clear();
    for (auto& v : buf) {
        collected.push_back(v);
    }
    assert((collected == std::vector<int>{3, 4, 5, 6, 7}));
    std::cout << "[pass] overwrite yields {3,4,5,6,7}\n";

    int sum = std::accumulate(buf.begin(), buf.end(), 0);
    assert(sum == 25);
    std::cout << "[pass] std::accumulate == 25\n";

    std::cout << "\nAll day-16 tests passed.\n";
    return 0;
}
