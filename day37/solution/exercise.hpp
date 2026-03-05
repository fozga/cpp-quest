#pragma once
#include <string>
#include <vector>

struct Logger {
    virtual ~Logger() = default;
    virtual void info(const std::string& msg) = 0;
};

class ConsoleLogger : public Logger {
public:
    void info(const std::string& msg) override;
};

struct LineItem {
    std::string name;
    int quantity = 0;
    int unit_price = 0;
};

class OrderProcessor {
public:
    explicit OrderProcessor(Logger& logger);
    int process_order(const std::vector<LineItem>& items);
private:
    Logger& logger_;
};
