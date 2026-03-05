#include "exercise.hpp"
#include <cassert>
#include <iostream>
#include <string>

int main() {
    {
        Any empty;
        assert(!empty.has_value());
        assert(empty.type() == typeid(void));
        std::cout << "[PASS] Default-constructed Any is empty" << std::endl;
    }

    {
        Any a(42);
        assert(a.has_value());
        assert(a.type() == typeid(int));
        assert(any_cast<int>(a) == 42);
        std::cout << "[PASS] Any(int) stores and retrieves 42" << std::endl;
    }

    {
        Any b(std::string("hello"));
        assert(b.has_value());
        assert(b.type() == typeid(std::string));
        assert(any_cast<std::string>(b) == "hello");
        std::cout << "[PASS] Any(string) stores and retrieves \"hello\"" << std::endl;
    }

    {
        Any b(std::string("hello"));
        bool caught = false;
        try {
            [[maybe_unused]] int v = any_cast<int>(b);
        } catch (const std::bad_cast&) {
            caught = true;
        }
        assert(caught);
        std::cout << "[PASS] any_cast type mismatch throws std::bad_cast" << std::endl;
    }

    {
        Any a(42);
        assert(any_cast<int>(&a) != nullptr);
        assert(*any_cast<int>(&a) == 42);
        assert(any_cast<double>(&a) == nullptr);
        std::cout << "[PASS] Pointer any_cast returns correct pointer or nullptr" << std::endl;
    }

    {
        Any a(42);
        Any c = a;
        assert(c.has_value());
        assert(c.type() == typeid(int));
        assert(any_cast<int>(c) == 42);
        assert(any_cast<int>(a) == 42);
        std::cout << "[PASS] Copy constructor deep-copies value" << std::endl;
    }

    {
        Any a(42);
        Any d = std::move(a);
        assert(d.has_value());
        assert(any_cast<int>(d) == 42);
        assert(!a.has_value());
        std::cout << "[PASS] Move constructor transfers ownership" << std::endl;
    }

    {
        Any d(3.14);
        assert(d.has_value());
        d.reset();
        assert(!d.has_value());
        std::cout << "[PASS] reset() clears stored value" << std::endl;
    }

    std::cout << "All day-29 tests passed." << std::endl;
    return 0;
}
