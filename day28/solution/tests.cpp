#include "exercise.hpp"
#include <thread>
#include <cassert>
#include <iostream>
#include <chrono>
#include <vector>

int main() {
    {
        ReadyFlag flag;
        assert(!flag.try_consume().has_value());

        std::jthread producer([&flag]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            flag.publish(42);
        });

        std::optional<int> val;
        while (!(val = flag.try_consume())) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        assert(val.value() == 42);
        std::cout << "[PASS] ReadyFlag: consumed value 42" << std::endl;
    }

    {
        RelaxedCounter counter;
        constexpr int THREADS = 4;
        constexpr int PER = 10000;
        std::vector<std::jthread> threads;
        for (int i = 0; i < THREADS; ++i) {
            threads.emplace_back([&counter]() {
                for (int j = 0; j < PER; ++j)
                    counter.increment();
            });
        }
        threads.clear();
        assert(counter.value() == THREADS * PER);
        std::cout << "[PASS] RelaxedCounter = " << counter.value() << std::endl;
    }

    std::cout << "All day-28 tests passed." << std::endl;
    return 0;
}
