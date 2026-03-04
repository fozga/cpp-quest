#include "exercise.hpp"
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    assert(get_live_file_logger_count() == 0);

    std::vector<BaseLogger*> loggers;
    loggers.push_back(make_file_logger("app.log"));
    loggers.push_back(make_file_logger("error.log"));
    loggers.push_back(make_console_logger());

    assert(get_live_file_logger_count() == 2);

    use_and_destroy_all(loggers);

    assert(get_live_file_logger_count() == 0);

    std::cout << "day 08 – vtables & memory leaks – all tests passed\n";
    return 0;
}
