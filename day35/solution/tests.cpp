#include "exercise.hpp"
#include <cassert>
#include <iostream>

int main() {
    CounterService svc;
    assert(svc.count() == 0);

    svc.add_counter("requests", 0);
    svc.add_counter("errors", 0);
    assert(svc.count() == 2);
    assert(svc.has("requests"));

    svc.increment("requests");
    svc.increment("requests");
    svc.increment("requests");
    assert(svc.get("requests") == 3);

    svc.increment("errors", 5);
    assert(svc.get("errors") == 5);

    svc.add_counter("latency_ms", 100);
    svc.increment("latency_ms", -20);
    assert(svc.get("latency_ms") == 80);

    // Unknown counter throws
    bool threw = false;
    try { svc.get("missing"); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    threw = false;
    try { svc.increment("missing"); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    std::cout << "[PASS] add_counter and get" << std::endl;
    std::cout << "[PASS] increment" << std::endl;
    std::cout << "[PASS] unknown counter throws" << std::endl;
    std::cout << "All day-35 tests passed." << std::endl;
    return 0;
}
