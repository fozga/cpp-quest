#include "exercise.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    // --- Basic aggregation ---
    auto agg = make_aggregator("temperature");
    agg.feed(10.0);
    agg.feed(20.0);
    agg.feed(30.0);

    auto snap = get_snapshot(agg);
    assert(snap.count == 3);
    assert(snap.sum == 60.0);
    assert(snap.min == 10.0);
    assert(snap.max == 30.0);
    assert(std::abs(snap.average - 20.0) < 1e-9);
    std::cout << "[PASS] basic aggregation\n";

    // --- Independent aggregators ---
    auto agg2 = make_aggregator("humidity");
    agg2.feed(55.0);
    agg2.feed(65.0);

    auto snap2 = get_snapshot(agg2);
    assert(snap2.count == 2);
    assert(snap2.sum == 120.0);
    assert(snap2.min == 55.0);
    assert(snap2.max == 65.0);
    assert(std::abs(snap2.average - 60.0) < 1e-9);

    auto snap_orig = get_snapshot(agg);
    assert(snap_orig.count == 3);
    std::cout << "[PASS] independent aggregators\n";

    // --- Empty aggregator ---
    auto agg3 = make_aggregator("empty");
    auto snap3 = get_snapshot(agg3);
    assert(snap3.count == 0);
    assert(snap3.average == 0.0);
    std::cout << "[PASS] empty aggregator\n";

    std::cout << "All day-15 tests passed.\n";
    return 0;
}
