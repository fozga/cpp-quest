#include "exercise.hpp"

void LockFreeCounter::increment() {
    int expected = counter_.load(std::memory_order_relaxed);
    while (!counter_.compare_exchange_weak(expected, expected + 1,
                                            std::memory_order_relaxed,
                                            std::memory_order_relaxed)) {
    }
}

void LockFreeCounter::add(int n) {
    int expected = counter_.load(std::memory_order_relaxed);
    while (!counter_.compare_exchange_weak(expected, expected + n,
                                            std::memory_order_relaxed,
                                            std::memory_order_relaxed)) {
    }
}

int LockFreeCounter::value() const noexcept {
    return counter_.load(std::memory_order_relaxed);
}
