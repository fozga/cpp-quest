#include "exercise.hpp"

void ReadyFlag::publish(int v) {
    value_ = v;
    ready_.store(true, std::memory_order_release);
}

std::optional<int> ReadyFlag::try_consume() {
    if (ready_.load(std::memory_order_acquire))
        return value_;
    return std::nullopt;
}

void RelaxedCounter::increment() noexcept {
    count_.fetch_add(1, std::memory_order_relaxed);
}

int RelaxedCounter::value() const noexcept {
    return count_.load(std::memory_order_relaxed);
}
