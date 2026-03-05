#include "exercise.hpp"
#include <ctime>
#include <sstream>
#include <iomanip>

std::string format_timestamp(std::chrono::system_clock::time_point tp) {
    const auto time_t_val = std::chrono::system_clock::to_time_t(tp);
    const auto* tm_ptr = std::localtime(&time_t_val);
    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M");
    return oss.str();
}

std::string format_duration_ms(std::chrono::milliseconds ms) {
    const auto total_seconds = ms.count() / 1000;
    const auto remaining_ms = ms.count() % 1000;
    std::ostringstream oss;
    oss << total_seconds << "." << std::setfill('0') << std::setw(3) << remaining_ms << "s";
    return oss.str();
}
