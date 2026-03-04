#include "exercise.hpp"
#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::cout << "[day23] Running SFINAE-to-Concepts tests...\n";

    assert(serialize_concept(42) == "int:42");
    std::cout << "  [pass] serialize_concept(42)\n";

    assert(serialize_concept(0u) == "int:0");
    std::cout << "  [pass] serialize_concept(0u)\n";

    assert(serialize_concept(true) == "int:1");
    std::cout << "  [pass] serialize_concept(true)\n";

    std::string float_result = serialize_concept(3.14);
    assert(float_result.rfind("float:", 0) == 0);
    std::cout << "  [pass] serialize_concept(3.14) starts with \"float:\"\n";

    assert(serialize_concept(std::string("hello")) == "str:hello");
    std::cout << "  [pass] serialize_concept(string)\n";

    assert(serialize_concept(-1) == "int:-1");
    std::cout << "  [pass] serialize_concept(-1)\n";

    std::string float_result2 = serialize_concept(2.0f);
    assert(float_result2.rfind("float:", 0) == 0);
    std::cout << "  [pass] serialize_concept(2.0f) starts with \"float:\"\n";

    // serialize_concept(std::vector<int>{}) would fail the concept check at compile time.

    std::cout << "[day23] All tests passed.\n";
    return 0;
}
