#pragma once

#include <string>
#include <vector>

class BaseLogger {
public:
    virtual void log(const std::string& msg) const;

    // CORRECTED: originally non-virtual, which caused memory leaks when
    // deleting derived objects (e.g. FileLogger) through a BaseLogger*.
    // Making the destructor virtual ensures dynamic dispatch calls the
    // most-derived destructor first, properly releasing all resources.
    virtual ~BaseLogger() noexcept;
};

class FileLogger : public BaseLogger {
private:
    std::string* filename_;

public:
    explicit FileLogger(const std::string& name);
    ~FileLogger() noexcept override;
    void log(const std::string& msg) const override;
};

class ConsoleLogger : public BaseLogger {
public:
    void log(const std::string& msg) const override;
    ~ConsoleLogger() noexcept override = default;
};

BaseLogger* make_file_logger(const std::string& name);
BaseLogger* make_console_logger();
void use_and_destroy_all(std::vector<BaseLogger*>& loggers);
int get_live_file_logger_count();
