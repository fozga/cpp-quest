#include "exercise.hpp"

#include <cassert>
#include <iostream>
#include <utility>
#include <string>

int main() {
    // -- Default constructor --------------------------------------------------
    {
        DynamicArray<int> a;
        assert(a.size() == 0);
        assert(a.empty());
        std::cout << "[PASS] Default constructor\n";
    }

    // -- Size constructor -----------------------------------------------------
    {
        DynamicArray<int> a(5);
        assert(a.size() == 5);
        assert(!a.empty());
        for (std::size_t i = 0; i < a.size(); ++i)
            assert(a[i] == 0);
        std::cout << "[PASS] Size constructor\n";
    }

    // -- Initializer-list constructor -----------------------------------------
    {
        DynamicArray<int> a{1, 2, 3};
        assert(a.size() == 3);
        assert(a[0] == 1);
        assert(a[1] == 2);
        assert(a[2] == 3);
        std::cout << "[PASS] Initializer-list constructor\n";
    }

    // -- Copy constructor -----------------------------------------------------
    {
        DynamicArray<int> a{10, 20, 30};
        DynamicArray<int> b = a;
        assert(b.size() == a.size());
        for (std::size_t i = 0; i < a.size(); ++i)
            assert(b[i] == a[i]);

        b[0] = 999;
        assert(a[0] == 10);
        std::cout << "[PASS] Copy constructor (deep, independent)\n";
    }

    // -- Copy-assignment operator ---------------------------------------------
    {
        DynamicArray<int> a{1, 2, 3};
        DynamicArray<int> b{100};
        b = a;
        assert(b.size() == 3);
        assert(b[0] == 1 && b[1] == 2 && b[2] == 3);

        b[2] = 42;
        assert(a[2] == 3);
        std::cout << "[PASS] Copy-assignment operator\n";
    }

    // -- Move constructor -----------------------------------------------------
    {
        DynamicArray<int> a{4, 5, 6};
        std::size_t original_size = a.size();

        DynamicArray<int> c = std::move(a);
        assert(c.size() == original_size);
        assert(c[0] == 4 && c[1] == 5 && c[2] == 6);

        assert(a.size() == 0);
        assert(a.empty());
        std::cout << "[PASS] Move constructor (source is empty)\n";
    }

    // -- Move-assignment operator ---------------------------------------------
    {
        DynamicArray<int> a{7, 8, 9};
        DynamicArray<int> d{0};

        d = std::move(a);
        assert(d.size() == 3);
        assert(d[0] == 7 && d[1] == 8 && d[2] == 9);

        assert(a.size() == 0);
        assert(a.empty());
        std::cout << "[PASS] Move-assignment operator\n";
    }

    // -- push_back (lvalue) ---------------------------------------------------
    {
        DynamicArray<int> a;
        int val = 42;
        a.push_back(val);
        assert(a.size() == 1);
        assert(a[0] == 42);

        a.push_back(val);
        a.push_back(val);
        assert(a.size() == 3);
        std::cout << "[PASS] push_back (lvalue)\n";
    }

    // -- push_back (rvalue) ---------------------------------------------------
    {
        DynamicArray<int> a;
        a.push_back(100);
        a.push_back(200);
        assert(a.size() == 2);
        assert(a[0] == 100);
        assert(a[1] == 200);
        std::cout << "[PASS] push_back (rvalue)\n";
    }

    // -- DynamicArray<std::string> — non-trivial type -------------------------
    {
        DynamicArray<std::string> sa{"hello", "world"};
        assert(sa.size() == 2);
        assert(sa[0] == "hello");
        assert(sa[1] == "world");

        DynamicArray<std::string> sb = std::move(sa);
        assert(sb.size() == 2);
        assert(sb[0] == "hello");
        assert(sa.size() == 0);
        assert(sa.empty());

        sb.push_back(std::string("foo"));
        assert(sb.size() == 3);
        assert(sb[2] == "foo");

        std::string bar = "bar";
        sb.push_back(bar);
        assert(sb.size() == 4);
        assert(sb[3] == "bar");

        std::cout << "[PASS] DynamicArray<std::string> moves\n";
    }

    std::cout << "\nAll Day 05 tests passed.\n";
    return 0;
}
