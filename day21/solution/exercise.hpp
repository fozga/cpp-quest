#pragma once
#include <type_traits>
#include <string>
#include <sstream>

// Integral overload
template <typename T>
std::enable_if_t<std::is_integral_v<T>, std::string>
serialize(T value) {
    std::ostringstream oss;
    oss << "int:" << value;
    return oss.str();
}

// Non-integral overload
template <typename T>
std::enable_if_t<!std::is_integral_v<T>, std::string>
serialize(const T& value) {
    std::ostringstream oss;
    oss << "other:" << value;
    return oss.str();
}
