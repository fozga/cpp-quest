#include "exercise.hpp"

#include <cassert>
#include <iostream>
#include <utility>

int main() {
    std::cout << "[test] Default FileHandle is invalid ... ";
    {
        FileHandle fh;
        assert(!fh.is_valid());
    }
    std::cout << "PASSED\n";

    std::cout << "[test] Construct from open_pseudo_file yields valid handle ... ";
    {
        FileHandle fh(open_pseudo_file("demo.txt"));
        assert(fh.is_valid());
        assert(fh.get() >= 0);
    }
    std::cout << "PASSED\n";

    std::cout << "[test] reset closes previous and acquires new ... ";
    {
        FileHandle fh(open_pseudo_file("a.txt"));
        int first = fh.get();
        fh.reset(open_pseudo_file("b.txt"));
        assert(fh.is_valid());
        assert(fh.get() != first);
    }
    std::cout << "PASSED\n";

    std::cout << "[test] reset() with no argument makes handle invalid ... ";
    {
        FileHandle fh(open_pseudo_file("c.txt"));
        assert(fh.is_valid());
        fh.reset();
        assert(!fh.is_valid());
    }
    std::cout << "PASSED\n";

    std::cout << "[test] Move construction transfers validity ... ";
    {
        FileHandle src(open_pseudo_file("src.txt"));
        int fd = src.get();
        FileHandle dst(std::move(src));
        assert(dst.is_valid());
        assert(dst.get() == fd);
        assert(!src.is_valid());
    }
    std::cout << "PASSED\n";

    std::cout << "[test] Move assignment transfers ownership ... ";
    {
        FileHandle a(open_pseudo_file("a2.txt"));
        FileHandle b(open_pseudo_file("b2.txt"));
        int fd_b = b.get();
        a = std::move(b);
        assert(a.is_valid());
        assert(a.get() == fd_b);
        assert(!b.is_valid());
    }
    std::cout << "PASSED\n";

    std::cout << "\nAll Day 01 tests passed.\n";
    return 0;
}
