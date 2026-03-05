#pragma once
#include <string>
#include <vector>

// --- Core module ---
int sum(const std::vector<int>& v);

// --- Util module ---
std::string join(const std::vector<int>& v, char separator);

// --- App module ---
std::string run_demo();
