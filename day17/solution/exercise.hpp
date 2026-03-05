#pragma once

#include <string>
#include <vector>
#include <cstddef>

std::vector<std::string> filter_and_uppercase_errors(const std::vector<std::string>& lines);

std::size_t count_non_empty_lines(const std::vector<std::string>& lines);
