#include "exercise.hpp"

#include <algorithm>
#include <charconv>
#include <cctype>

namespace {

std::string trim(std::string_view sv) {
    while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.front())))
        sv.remove_prefix(1);
    while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.back())))
        sv.remove_suffix(1);
    return std::string{sv};
}

std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

} // namespace

Config make_config_from_lines(const std::vector<std::string>& lines) {
    Config cfg;
    for (const auto& line : lines) {
        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;
        std::string key = trim(std::string_view{line}.substr(0, pos));
        std::string value = trim(std::string_view{line}.substr(pos + 1));
        if (key.empty())
            continue;
        cfg.entries.push_back(ConfigEntry{std::move(key), std::move(value)});
    }
    return cfg;
}

std::optional<std::string> get_string(const Config& cfg, std::string_view key) {
    for (const auto& entry : cfg.entries) {
        if (entry.key == key)
            return entry.value;
    }
    return std::nullopt;
}

std::optional<int> get_int(const Config& cfg, std::string_view key) {
    auto str = get_string(cfg, key);
    if (!str)
        return std::nullopt;

    const auto& s = *str;
    int val{};
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), val);
    if (ec != std::errc{} || ptr != s.data() + s.size())
        return std::nullopt;
    return val;
}

std::optional<bool> get_bool(const Config& cfg, std::string_view key) {
    auto str = get_string(cfg, key);
    if (!str)
        return std::nullopt;

    std::string lower = to_lower(*str);
    if (lower == "true" || lower == "1")
        return true;
    if (lower == "false" || lower == "0")
        return false;
    return std::nullopt;
}
