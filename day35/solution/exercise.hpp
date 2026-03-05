#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>

class CounterService {
public:
    CounterService() = default;

    void add_counter(std::string name, int initial = 0);
    void increment(const std::string& name, int delta = 1);
    int get(const std::string& name) const;
    bool has(const std::string& name) const noexcept;
    std::size_t count() const noexcept;

private:
    std::unordered_map<std::string, int> counters_;
};
