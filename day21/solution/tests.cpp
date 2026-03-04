#include "exercise.hpp"
#include <cassert>
#include <iostream>
#include <string>

int main() {
    assert(serialize(42) == "int:42");
    assert(serialize(0) == "int:0");
    assert(serialize(-1) == "int:-1");
    assert(serialize(1u) == "int:1");
    assert(serialize(true) == "int:1");
    assert(serialize(3.14) == "other:3.14");
    assert(serialize(std::string("hello")) == "other:hello");

    std::cout << "Day 21: SFINAE Constraints — all tests passed!\n";
    return 0;
}
