#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

struct ConfigEntry {
    std::string key;
    std::string value;
};

struct Config {
    std::vector<ConfigEntry> entries;
};

Config make_config_from_lines(const std::vector<std::string>& lines);

std::optional<std::string> get_string(const Config& cfg, std::string_view key);

std::optional<int> get_int(const Config& cfg, std::string_view key);

std::optional<bool> get_bool(const Config& cfg, std::string_view key);
