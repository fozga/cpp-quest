#include "exercise.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

int main() {
    std::cout << "=== Day 07: Perfect Forwarding Tests ===\n\n";

    reset_log_count();

    // 1. make_with_log<Widget> with rvalue arguments
    {
        auto w1 = make_with_log<Widget>(std::string("test"), 42);
        assert(w1.name == "test");
        assert(w1.value == 42);
        std::cout << "[PASS] make_with_log<Widget> with rvalue args\n";
    }

    // 2. make_with_log<Widget> with lvalue arguments (must not move from them)
    {
        std::string name = "hello";
        auto w2 = make_with_log<Widget>(name, 10);
        assert(w2.name == "hello");
        assert(w2.value == 10);
        assert(name == "hello");  // lvalue must not be moved from
        std::cout << "[PASS] make_with_log<Widget> with lvalue args (original preserved)\n";
    }

    // 3. make_with_log<std::string> with const char*
    {
        auto s = make_with_log<std::string>("world");
        assert(s == "world");
        std::cout << "[PASS] make_with_log<std::string> with const char*\n";
    }

    // 4. call_with_log with lambda returning int
    {
        auto result = call_with_log([](int a, int b) { return a + b; }, 3, 4);
        assert(result == 7);
        std::cout << "[PASS] call_with_log with lambda returning int\n";
    }

    // 5. call_with_log with lambda modifying lvalue via reference
    {
        int x = 10;
        call_with_log([](int& v) { v *= 2; }, x);
        assert(x == 20);
        std::cout << "[PASS] call_with_log with lvalue-ref mutation\n";
    }

    // 6. Verify total log count matches number of forwarding calls
    {
        assert(get_log_count() == 5);
        std::cout << "[PASS] log count is correct (" << get_log_count() << ")\n";
    }

    std::cout << "\nAll tests passed.\n";
    return 0;
}
