#include "exercise.hpp"

void transfer(Account& from, Account& to, int amount) {
    std::scoped_lock lock(from.mtx, to.mtx);
    if (from.balance >= amount) {
        from.balance -= amount;
        to.balance += amount;
    }
}

int total_balance(const std::vector<std::unique_ptr<Account>>& accounts) {
    int sum = 0;
    for (const auto& acc : accounts) {
        std::lock_guard<std::mutex> lk(acc->mtx);
        sum += acc->balance;
    }
    return sum;
}
