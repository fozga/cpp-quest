#pragma once
#include <string>
#include <chrono>

std::string format_timestamp(std::chrono::system_clock::time_point tp);
std::string format_duration_ms(std::chrono::milliseconds ms);
