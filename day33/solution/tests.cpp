#include "exercise.hpp"
#include <cassert>
#include <iostream>
#include <utility>

int main() {
    // Basic usage
    {
        Buffer buf(10);
        assert(buf.size() == 0);
        assert(buf.capacity() == 10);
        for (int i = 0; i < 10; ++i) buf.push_back(i);
        assert(buf.size() == 10);
        for (std::size_t i = 0; i < buf.size(); ++i)
            assert(buf[i] == static_cast<int>(i));
        std::cout << "[PASS] basic push_back and operator[]\n";
    }

    // Move construction
    {
        Buffer a(5);
        a.push_back(42);
        a.push_back(99);
        Buffer b = std::move(a);
        assert(b.size() == 2);
        assert(b[0] == 42);
        assert(b[1] == 99);
        assert(a.size() == 0);
        assert(a.capacity() == 0);
        std::cout << "[PASS] move constructor\n";
    }

    // Move assignment
    {
        Buffer a(3);
        a.push_back(1);
        Buffer b(2);
        b = std::move(a);
        assert(b.size() == 1);
        assert(b[0] == 1);
        assert(a.size() == 0);
        std::cout << "[PASS] move assignment\n";
    }

    std::cout << "Day33 Buffer tests passed (run asan/valgrind to confirm no memory issues).\n";
    return 0;
}
