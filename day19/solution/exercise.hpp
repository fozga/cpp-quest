#pragma once
#include <string>
#include <type_traits>

template <typename T>
struct is_std_string : std::false_type {};

template <typename Traits, typename Alloc>
struct is_std_string<std::basic_string<char, Traits, Alloc>> : std::true_type {};

template <typename T>
inline constexpr bool is_std_string_v = is_std_string<T>::value;

template <typename T>
constexpr bool check_is_std_string() {
    return is_std_string_v<T>;
}
