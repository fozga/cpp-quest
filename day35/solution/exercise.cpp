#include "exercise.hpp"

void CounterService::add_counter(std::string name, int initial) {
    counters_[std::move(name)] = initial;
}

void CounterService::increment(const std::string& name, int delta) {
    auto it = counters_.find(name);
    if (it == counters_.end())
        throw std::out_of_range("Counter not found: " + name);
    it->second += delta;
}

int CounterService::get(const std::string& name) const {
    return counters_.at(name);
}

bool CounterService::has(const std::string& name) const noexcept {
    return counters_.count(name) > 0;
}

std::size_t CounterService::count() const noexcept {
    return counters_.size();
}
