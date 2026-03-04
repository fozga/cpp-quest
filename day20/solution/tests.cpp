#include "exercise.hpp"
#include <sstream>
#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::cout << "[day20] Running variadic template tests...\n";

    std::ostringstream oss;
    log_line(oss, "Answer:", 42, 3.14);
    std::string s = oss.str();

    assert(s.find("Answer:") != std::string::npos);
    assert(s.find("42") != std::string::npos);
    assert(s.find("3.14") != std::string::npos);
    assert(!s.empty() && s.back() == '\n');

    assert(arg_count(1, 2, 3) == 3);
    assert(arg_count() == 0);

    log_to_cout("Hello", "variadic", "world");

    std::cout << "[day20] All tests passed.\n";
    return 0;
}
