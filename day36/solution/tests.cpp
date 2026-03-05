#include "exercise.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
    StringInterner si;
    assert(si.size() == 0);

    auto id_foo = si.intern("foo");
    auto id_bar = si.intern("bar");
    auto id_foo2 = si.intern("foo");
    assert(id_foo == id_foo2);
    assert(si.size() == 2);

    assert(si.has_string("foo"));
    assert(si.has_string("bar"));
    assert(!si.has_string("baz"));

    assert(si.has_id(id_foo));
    assert(si.has_id(id_bar));
    assert(!si.has_id(999));

    assert(si.get_string(id_foo) == "foo");
    assert(si.get_string(id_bar) == "bar");
    assert(si.get_id("foo") == id_foo);
    assert(si.get_id("bar") == id_bar);

    bool threw = false;
    try { si.get_id("missing"); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    std::cout << "[PASS] intern, lookup, and error handling" << std::endl;
    std::cout << "All day-36 tests passed." << std::endl;
    return 0;
}
