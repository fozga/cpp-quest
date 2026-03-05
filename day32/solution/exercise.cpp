#include "exercise.hpp"
#include <algorithm>
#include <cctype>

std::string trim(std::string_view s) {
    auto first = s.begin();
    while (first != s.end() && std::isspace(static_cast<unsigned char>(*first)))
        ++first;

    if (first == s.end())
        return "";

    auto last = s.end();
    do {
        --last;
    } while (last != first && std::isspace(static_cast<unsigned char>(*last)));

    return std::string(first, last + 1);
}

std::string to_upper(std::string_view s) {
    std::string result(s);
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return result;
}

std::pair<std::string, std::string> split_once(std::string_view s, char delimiter) {
    auto pos = s.find(delimiter);
    if (pos == std::string_view::npos)
        return {std::string(s), ""};
    return {std::string(s.substr(0, pos)), std::string(s.substr(pos + 1))};
}
