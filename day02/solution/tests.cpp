#include "exercise.hpp"

#include <cassert>
#include <iostream>
#include <utility>

int main() {
    // 1. make_file_handle yields a valid handle
    {
        FileHandlePtr h = make_file_handle("alpha.txt");
        assert(is_valid_handle(h));
        assert(get_handle(h) >= 0);
        std::cout << "PASS: make_file_handle yields valid handle\n";
    }

    // 2. Moving transfers ownership; original becomes null
    {
        FileHandlePtr a = make_file_handle("beta.txt");
        int fd = get_handle(a);
        FileHandlePtr b = std::move(a);
        assert(!a);
        assert(is_valid_handle(b));
        assert(get_handle(b) == fd);
        std::cout << "PASS: move transfers ownership\n";
    }

    // 3. Scope exit invokes deleter — verify via close count
    {
        int before = get_close_count();
        {
            FileHandlePtr h = make_file_handle("gamma.txt");
            (void)h;
        }
        int after = get_close_count();
        assert(after == before + 1);
        std::cout << "PASS: scope exit invokes deleter\n";
    }

    // 4. Multiple handles — independent cleanup
    {
        int before = get_close_count();
        {
            FileHandlePtr h1 = make_file_handle("delta.txt");
            FileHandlePtr h2 = make_file_handle("epsilon.txt");
            FileHandlePtr h3 = make_file_handle("zeta.txt");
            assert(is_valid_handle(h1));
            assert(is_valid_handle(h2));
            assert(is_valid_handle(h3));
        }
        int after = get_close_count();
        assert(after == before + 3);
        std::cout << "PASS: multiple handles cleaned up independently\n";
    }

    std::cout << "\nAll Day 02 tests passed.\n";
    return 0;
}
