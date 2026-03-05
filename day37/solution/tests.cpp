#include "exercise.hpp"
#include <cassert>
#include <iostream>
#include <string>

struct VectorLogger : Logger {
    std::vector<std::string> messages;
    void info(const std::string& msg) override { messages.push_back(msg); }
};

int main() {
    VectorLogger log;
    OrderProcessor proc{log};

    std::vector<LineItem> items{
        {"Widget", 3, 100},
        {"Gadget", 2, 250},
    };

    int total = proc.process_order(items);
    assert(total == 3 * 100 + 2 * 250);
    assert(log.messages.size() >= 2);

    bool found_processing = false, found_total = false;
    for (const auto& m : log.messages) {
        if (m.find("Processing") != std::string::npos) found_processing = true;
        if (m.find("800") != std::string::npos) found_total = true;
    }
    assert(found_processing);
    assert(found_total);

    std::cout << "[PASS] process_order total correct" << std::endl;
    std::cout << "[PASS] logger received expected messages" << std::endl;
    std::cout << "All day-37 tests passed." << std::endl;
    return 0;
}
