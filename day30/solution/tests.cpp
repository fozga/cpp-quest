#include "exercise.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <string>

int main() {
    // Small type — should be inline
    {
        SmallAny a(42);
        assert(a.has_value());
        assert(a.type() == typeid(int));
        assert(a.is_inline());
        assert(any_cast<int>(a) == 42);
        std::cout << "[PASS] int stored inline" << std::endl;
    }
    // Small double
    {
        SmallAny a(3.14);
        assert(a.is_inline());
        assert(any_cast<double>(a) == 3.14);
        std::cout << "[PASS] double stored inline" << std::endl;
    }
    // Large type — should be on heap
    {
        std::string big(200, 'x');
        SmallAny a(big);
        assert(a.has_value());
        assert(!a.is_inline());
        assert(any_cast<std::string>(a) == big);
        std::cout << "[PASS] large string stored on heap" << std::endl;
    }
    // Copy (inline)
    {
        SmallAny a(42);
        SmallAny b = a;
        assert(any_cast<int>(b) == 42);
        assert(b.is_inline());
        std::cout << "[PASS] copy inline" << std::endl;
    }
    // Copy (heap)
    {
        SmallAny a(std::string(200, 'y'));
        SmallAny b = a;
        assert(any_cast<std::string>(b) == std::string(200, 'y'));
        std::cout << "[PASS] copy heap" << std::endl;
    }
    // Move (inline)
    {
        SmallAny a(42);
        SmallAny b = std::move(a);
        assert(b.has_value());
        assert(!a.has_value());
        assert(any_cast<int>(b) == 42);
        std::cout << "[PASS] move inline" << std::endl;
    }
    // Move (heap)
    {
        SmallAny a(std::string(200, 'z'));
        SmallAny b = std::move(a);
        assert(b.has_value());
        assert(!a.has_value());
        assert(any_cast<std::string>(b) == std::string(200, 'z'));
        std::cout << "[PASS] move heap" << std::endl;
    }
    // Type mismatch
    {
        SmallAny a(42);
        bool threw = false;
        try {
            any_cast<std::string>(a);
        } catch (const std::bad_cast&) {
            threw = true;
        }
        assert(threw);
        std::cout << "[PASS] type mismatch throws" << std::endl;
    }
    // Empty
    {
        SmallAny a;
        assert(!a.has_value());
        std::cout << "[PASS] empty SmallAny" << std::endl;
    }

    std::cout << "All day-30 tests passed." << std::endl;
    return 0;
}
