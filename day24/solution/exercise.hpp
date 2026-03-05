#pragma once
#include <thread>
#include <chrono>
#include <cstddef>
#include <functional>
#include <atomic>

class PeriodicWorker {
public:
    using Duration = std::chrono::milliseconds;

    explicit PeriodicWorker(Duration interval, std::function<void()> task);
    ~PeriodicWorker();

    PeriodicWorker(const PeriodicWorker&) = delete;
    PeriodicWorker& operator=(const PeriodicWorker&) = delete;

    std::size_t iterations() const noexcept;

private:
    Duration interval_;
    std::function<void()> task_;
    std::atomic<std::size_t> iterations_{0};
    std::jthread worker_;
};
