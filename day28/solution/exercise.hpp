#pragma once
#include <atomic>
#include <optional>

class ReadyFlag {
public:
    ReadyFlag() noexcept : ready_(false), value_(0) {}

    void publish(int v);
    std::optional<int> try_consume();

private:
    std::atomic<bool> ready_;
    int value_;
};

class RelaxedCounter {
public:
    RelaxedCounter() noexcept : count_(0) {}

    void increment() noexcept;
    int value() const noexcept;

private:
    std::atomic<int> count_;
};
