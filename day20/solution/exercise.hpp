#pragma once
#include <ostream>
#include <iostream>

template <typename... Ts>
void log_line(std::ostream& os, const Ts&... args) {
    ((os << args << ' '), ...);
    os << '\n';
}

template <typename... Ts>
void log_to_cout(const Ts&... args) {
    log_line(std::cout, args...);
}

template <typename... Ts>
std::size_t arg_count(const Ts&...) {
    return sizeof...(Ts);
}
