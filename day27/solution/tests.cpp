#include "exercise.hpp"
#include <thread>
#include <vector>
#include <cassert>
#include <iostream>

int main() {
    LockFreeCounter c;
    assert(c.value() == 0);

    constexpr int THREADS = 4;
    constexpr int PER_THREAD = 10000;

    std::vector<std::jthread> threads;
    for (int t = 0; t < THREADS; ++t) {
        threads.emplace_back([&c]() {
            for (int i = 0; i < PER_THREAD; ++i)
                c.increment();
        });
    }
    threads.clear();

    assert(c.value() == THREADS * PER_THREAD);
    std::cout << "[PASS] Counter = " << c.value()
              << " (expected " << THREADS * PER_THREAD << ")" << std::endl;

    LockFreeCounter c2;
    c2.add(100);
    c2.add(200);
    assert(c2.value() == 300);
    std::cout << "[PASS] add() works" << std::endl;

    std::cout << "All day-27 tests passed." << std::endl;
    return 0;
}
