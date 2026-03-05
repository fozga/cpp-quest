#pragma once
#include <future>
#include <vector>

std::future<int> async_accumulate(const std::vector<int>& values);
std::future<long long> async_accumulate_ll(const std::vector<int>& values);
