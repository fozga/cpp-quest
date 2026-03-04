#include "exercise.hpp"
#include <numeric>
#include <thread>

std::future<int> async_accumulate(const std::vector<int>& values) {
    auto prom = std::make_shared<std::promise<int>>();
    auto fut = prom->get_future();
    std::thread([prom, vals = values]() {
        int sum = std::accumulate(vals.begin(), vals.end(), 0);
        prom->set_value(sum);
    }).detach();
    return fut;
}

std::future<long long> async_accumulate_ll(const std::vector<int>& values) {
    auto prom = std::make_shared<std::promise<long long>>();
    auto fut = prom->get_future();
    std::thread([prom, vals = values]() {
        long long sum = std::accumulate(vals.begin(), vals.end(), 0LL);
        prom->set_value(sum);
    }).detach();
    return fut;
}
