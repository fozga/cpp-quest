#pragma once

#include <string>
#include <functional>
#include <limits>
#include <memory>
#include <cstddef>

struct AggregatorState {
    std::size_t count = 0;
    double sum = 0.0;
    double min = std::numeric_limits<double>::infinity();
    double max = -std::numeric_limits<double>::infinity();
};

struct AggregatorSnapshot {
    std::size_t count;
    double sum;
    double min;
    double max;
    double average;
};

struct AggregatorImpl {
    AggregatorState state;
    std::string label;
};

struct AggregatorHandle {
    std::function<void(double)> feed;
    std::shared_ptr<AggregatorImpl> impl;
};

AggregatorHandle make_aggregator(const std::string& label);
AggregatorSnapshot get_snapshot(const AggregatorHandle& agg);
