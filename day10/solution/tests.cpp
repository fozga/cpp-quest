#include "exercise.hpp"

#include <cassert>
#include <iostream>
#include <string>

int main() {
    IODevice dev("tty0");

    assert(dev.name() == "tty0");

    Device& base_ref = dev;
    assert(base_ref.name() == "tty0");

    std::string r = dev.read();
    assert(r.find("IODevice") != std::string::npos);
    assert(r.find("tty0") != std::string::npos);

    demo_io_device(dev);

    InputDevice in("stdin");
    assert(in.name() == "stdin");
    assert(in.read().find("InputDevice") != std::string::npos);

    OutputDevice out("stdout");
    assert(out.name() == "stdout");
    out.write("test output");

    std::cout << "\n[day10] All tests passed.\n";
    return 0;
}
