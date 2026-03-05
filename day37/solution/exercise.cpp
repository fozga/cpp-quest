#include "exercise.hpp"
#include <iostream>
#include <sstream>

void ConsoleLogger::info(const std::string& msg) {
    std::cout << "INFO: " << msg << '\n';
}

OrderProcessor::OrderProcessor(Logger& logger) : logger_(logger) {}

int OrderProcessor::process_order(const std::vector<LineItem>& items) {
    std::ostringstream oss;
    oss << "Processing order with " << items.size() << " items";
    logger_.info(oss.str());

    int total = 0;
    for (const auto& item : items)
        total += item.quantity * item.unit_price;

    oss.str("");
    oss << "Order total: " << total;
    logger_.info(oss.str());

    return total;
}
