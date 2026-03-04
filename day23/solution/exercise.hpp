#pragma once
#include <string>
#include <type_traits>
#include <sstream>

template <typename T>
concept IntegralSerializable = std::is_integral_v<T>;

template <typename T>
concept FloatingSerializable = std::is_floating_point_v<T>;

template <IntegralSerializable T>
std::string serialize_concept(T value) {
    std::ostringstream oss;
    oss << "int:" << value;
    return oss.str();
}

template <FloatingSerializable T>
std::string serialize_concept(T value) {
    std::ostringstream oss;
    oss << "float:" << value;
    return oss.str();
}

std::string serialize_concept(const std::string& value);
