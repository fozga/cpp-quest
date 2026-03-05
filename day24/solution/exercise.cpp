#include "exercise.hpp"

PeriodicWorker::PeriodicWorker(Duration interval, std::function<void()> task)
    : interval_(interval), task_(std::move(task)),
      worker_([this](std::stop_token st) {
          while (!st.stop_requested()) {
              std::this_thread::sleep_for(interval_);
              if (st.stop_requested()) break;
              task_();
              iterations_.fetch_add(1, std::memory_order_relaxed);
          }
      })
{}

PeriodicWorker::~PeriodicWorker() {
    worker_.request_stop();
}

std::size_t PeriodicWorker::iterations() const noexcept {
    return iterations_.load(std::memory_order_relaxed);
}
