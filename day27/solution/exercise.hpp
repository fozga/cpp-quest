#pragma once
#include <atomic>

class LockFreeCounter {
public:
    LockFreeCounter() noexcept : counter_(0) {}

    void increment();
    void add(int n);
    int value() const noexcept;

private:
    std::atomic<int> counter_;
};
