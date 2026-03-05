#pragma once
#include <string>
#include <string_view>
#include <utility>

std::string trim(std::string_view s);
std::string to_upper(std::string_view s);
std::pair<std::string, std::string> split_once(std::string_view s, char delimiter);
