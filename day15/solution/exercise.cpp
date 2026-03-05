#include "exercise.hpp"

#include <algorithm>

AggregatorHandle make_aggregator(const std::string& label) {
    auto impl = std::make_shared<AggregatorImpl>();
    impl->label = label;

    auto feeder = [impl = impl](double value) mutable {
        auto& s = impl->state;
        s.count++;
        s.sum += value;
        if (value < s.min) s.min = value;
        if (value > s.max) s.max = value;
    };

    return AggregatorHandle{std::function<void(double)>(feeder), impl};
}

AggregatorSnapshot get_snapshot(const AggregatorHandle& agg) {
    const auto& s = agg.impl->state;
    double average = s.count > 0 ? s.sum / static_cast<double>(s.count) : 0.0;
    return AggregatorSnapshot{s.count, s.sum, s.min, s.max, average};
}
