#include "exercise.hpp"

#include <algorithm>
#include <cctype>
#include <ranges>

static std::string to_upper(std::string s) {
    for (auto& c : s)
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

std::vector<std::string> filter_and_uppercase_errors(const std::vector<std::string>& lines) {
    auto view = lines
        | std::views::filter([](const std::string& s) {
            return s.find("ERROR") != std::string::npos;
        })
        | std::views::transform([](const std::string& s) {
            return to_upper(s);
        });

    std::vector<std::string> result;
    for (auto&& s : view)
        result.push_back(std::string(s));
    return result;
}

std::size_t count_non_empty_lines(const std::vector<std::string>& lines) {
    return static_cast<std::size_t>(
        std::ranges::count_if(lines, [](const std::string& s) { return !s.empty(); })
    );
}
