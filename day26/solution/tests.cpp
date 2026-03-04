#include "exercise.hpp"
#include <thread>
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    constexpr int NUM_ACCOUNTS = 4;
    constexpr int INITIAL = 1000;
    constexpr int TRANSFERS = 5000;

    std::vector<std::unique_ptr<Account>> accounts;
    for (int i = 0; i < NUM_ACCOUNTS; ++i)
        accounts.push_back(std::make_unique<Account>(INITIAL));

    int expected_total = NUM_ACCOUNTS * INITIAL;
    assert(total_balance(accounts) == expected_total);

    std::vector<std::jthread> threads;
    for (int t = 0; t < 4; ++t) {
        threads.emplace_back([&accounts, t]() {
            for (int i = 0; i < TRANSFERS; ++i) {
                auto& from = *accounts[static_cast<std::size_t>((t + i) % static_cast<int>(accounts.size()))];
                auto& to = *accounts[static_cast<std::size_t>((t + i + 1) % static_cast<int>(accounts.size()))];
                transfer(from, to, 1);
            }
        });
    }
    threads.clear();

    int final_total = total_balance(accounts);
    assert(final_total == expected_total);
    std::cout << "[PASS] Total balance preserved: " << final_total << std::endl;
    std::cout << "All day-26 tests passed." << std::endl;
    return 0;
}
