#pragma once
#include <mutex>
#include <cstddef>
#include <memory>
#include <vector>

struct Account {
    mutable std::mutex mtx;
    int balance{0};
    explicit Account(int b) : balance(b) {}
};

void transfer(Account& from, Account& to, int amount);

int total_balance(const std::vector<std::unique_ptr<Account>>& accounts);
