#include "exercise.hpp"
#include <cassert>
#include <iostream>
#include <string>

int main() {
    static_assert(is_std_string_v<std::string>);
    static_assert(is_std_string_v<std::basic_string<char>>);
    static_assert(!is_std_string_v<std::wstring>);
    static_assert(!is_std_string_v<const char*>);
    static_assert(!is_std_string_v<int>);
    static_assert(!is_std_string_v<double>);

    std::cout << "[test 1] is_std_string_v<std::string> == true ... ";
    assert(check_is_std_string<std::string>());
    std::cout << "PASSED\n";

    std::cout << "[test 2] is_std_string_v<std::basic_string<char>> == true ... ";
    assert(check_is_std_string<std::basic_string<char>>());
    std::cout << "PASSED\n";

    std::cout << "[test 3] is_std_string_v<std::wstring> == false ... ";
    assert(!check_is_std_string<std::wstring>());
    std::cout << "PASSED\n";

    std::cout << "[test 4] is_std_string_v<const char*> == false ... ";
    assert(!check_is_std_string<const char*>());
    std::cout << "PASSED\n";

    std::cout << "[test 5] is_std_string_v<int> == false ... ";
    assert(!check_is_std_string<int>());
    std::cout << "PASSED\n";

    std::cout << "[test 6] is_std_string_v<double> == false ... ";
    assert(!check_is_std_string<double>());
    std::cout << "PASSED\n";

    std::cout << "\nAll type-trait tests passed!\n";
    return 0;
}
