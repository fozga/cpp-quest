#include "exercise.hpp"
#include <iostream>

static int live_file_logger_count = 0;

void BaseLogger::log(const std::string& msg) const {
    std::cout << "[BaseLogger] " << msg << "\n";
}

BaseLogger::~BaseLogger() = default;

FileLogger::FileLogger(const std::string& name)
    : filename_(new std::string(name)) {
    ++live_file_logger_count;
}

FileLogger::~FileLogger() noexcept {
    delete filename_;
    --live_file_logger_count;
}

void FileLogger::log(const std::string& msg) const {
    std::cout << "[FileLogger:" << *filename_ << "] " << msg << "\n";
}

void ConsoleLogger::log(const std::string& msg) const {
    std::cout << "[ConsoleLogger] " << msg << "\n";
}

BaseLogger* make_file_logger(const std::string& name) {
    return new FileLogger(name);
}

BaseLogger* make_console_logger() {
    return new ConsoleLogger();
}

void use_and_destroy_all(std::vector<BaseLogger*>& loggers) {
    for (BaseLogger* logger : loggers) {
        logger->log("test");
        delete logger;
    }
    loggers.clear();
}

int get_live_file_logger_count() {
    return live_file_logger_count;
}
