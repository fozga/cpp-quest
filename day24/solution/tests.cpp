#include "exercise.hpp"
#include <cassert>
#include <iostream>
#include <atomic>

int main() {
    std::atomic<bool> flag{false};
    {
        PeriodicWorker worker{PeriodicWorker::Duration{20}, [&]{ flag.store(true); }};
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        auto iters = worker.iterations();
        std::cout << "Iterations: " << iters << std::endl;
        assert(iters > 0);
        assert(flag.load());
        std::cout << "[PASS] Worker ran and flag was set" << std::endl;
    }
    std::cout << "[PASS] Worker destroyed cleanly (jthread auto-joined)" << std::endl;
    std::cout << "All day-24 tests passed." << std::endl;
    return 0;
}
