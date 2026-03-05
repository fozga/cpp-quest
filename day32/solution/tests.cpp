#include "exercise.hpp"
#include <cassert>
#include <iostream>

int main() {
    // --- trim: removes leading and trailing whitespace ---
    {
        std::string input = " hello ";
        auto result = trim(input);
        assert(result == "hello");
    }
    std::cout << "[pass] trim: leading and trailing spaces\n";

    {
        std::string input = "   ";
        auto result = trim(input);
        assert(result == "");
    }
    std::cout << "[pass] trim: all spaces\n";

    {
        std::string input = "";
        auto result = trim(input);
        assert(result == "");
    }
    std::cout << "[pass] trim: empty string\n";

    {
        std::string input = "abc";
        auto result = trim(input);
        assert(result == "abc");
    }
    std::cout << "[pass] trim: no whitespace\n";

    // --- to_upper: converts to uppercase ---
    {
        std::string input = "Hello World";
        auto result = to_upper(input);
        assert(result == "HELLO WORLD");
    }
    std::cout << "[pass] to_upper: mixed case\n";

    {
        std::string input = "ABC";
        auto result = to_upper(input);
        assert(result == "ABC");
    }
    std::cout << "[pass] to_upper: already uppercase\n";

    {
        std::string input = "";
        auto result = to_upper(input);
        assert(result == "");
    }
    std::cout << "[pass] to_upper: empty string\n";

    // --- split_once: splits at first delimiter ---
    {
        auto result = split_once("key=value", '=');
        assert(result.first == "key");
        assert(result.second == "value");
    }
    std::cout << "[pass] split_once: basic split\n";

    {
        auto result = split_once("nodelim", '=');
        assert(result.first == "nodelim");
        assert(result.second == "");
    }
    std::cout << "[pass] split_once: no delimiter found\n";

    {
        auto result = split_once("=value", '=');
        assert(result.first == "");
        assert(result.second == "value");
    }
    std::cout << "[pass] split_once: delimiter at start\n";

    {
        auto result = split_once("a=b=c", '=');
        assert(result.first == "a");
        assert(result.second == "b=c");
    }
    std::cout << "[pass] split_once: multiple delimiters\n";

    std::cout << "\nAll tests passed!\n";
    return 0;
}
