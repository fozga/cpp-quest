#include "exercise.hpp"

#include <cassert>
#include <iostream>

int main() {
    std::vector<std::string> lines = {
        "INFO: start",
        "ERROR: something failed",
        "",
        "ERROR: another issue",
        "DEBUG: trace"
    };

    auto errors = filter_and_uppercase_errors(lines);
    assert(errors.size() == 2);
    assert(errors[0].find("ERROR") != std::string::npos);
    assert(errors[0] == "ERROR: SOMETHING FAILED");
    assert(errors[1] == "ERROR: ANOTHER ISSUE");

    assert(count_non_empty_lines(lines) == 4);

    std::cout << "Day 17: All tests passed!" << std::endl;
    return 0;
}
