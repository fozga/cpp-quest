#pragma once

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

struct Widget {
    std::string name;
    int value;

    Widget();
    Widget(std::string n, int v);
};

void increment_log_count();
int get_log_count();
void reset_log_count();

template <typename T, typename... Args>
T make_with_log(Args&&... args) {
    increment_log_count();
    std::cout << "[make_with_log] constructing object\n";
    return T(std::forward<Args>(args)...);
}

template <typename F, typename... Args>
decltype(auto) call_with_log(F&& f, Args&&... args) {
    increment_log_count();
    std::cout << "[call_with_log] invoking callable\n";
    return std::forward<F>(f)(std::forward<Args>(args)...);
}
