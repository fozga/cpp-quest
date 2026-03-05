#include "exercise.hpp"
#include <cassert>
#include <iostream>
#include <chrono>

int main() {
    // Test format_timestamp produces expected pattern length and separators
    {
        auto epoch = std::chrono::system_clock::from_time_t(0);
        auto result = format_timestamp(epoch);
        // "YYYY-MM-DD HH:MM" = 16 chars
        assert(result.size() == 16);
        assert(result[4] == '-');
        assert(result[7] == '-');
        assert(result[10] == ' ');
        assert(result[13] == ':');
        std::cout << "[PASS] format_timestamp pattern: " << result << std::endl;
    }
    // Test format_duration_ms
    {
        using namespace std::chrono_literals;
        assert(format_duration_ms(0ms) == "0.000s");
        assert(format_duration_ms(1500ms) == "1.500s");
        assert(format_duration_ms(42ms) == "0.042s");
        assert(format_duration_ms(60000ms) == "60.000s");
        std::cout << "[PASS] format_duration_ms" << std::endl;
    }
    std::cout << "All day-34 tests passed." << std::endl;
    return 0;
}
