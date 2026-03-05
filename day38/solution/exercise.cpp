#include "exercise.hpp"
#include <numeric>
#include <sstream>

int sum(const std::vector<int>& v) {
    return std::accumulate(v.begin(), v.end(), 0);
}

std::string join(const std::vector<int>& v, char separator) {
    std::ostringstream oss;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i > 0) oss << separator;
        oss << v[i];
    }
    return oss.str();
}

std::string run_demo() {
    std::vector<int> data{1, 2, 3};
    int s = sum(data);
    std::string j = join(data, ',');
    std::ostringstream oss;
    oss << "sum=" << s << "; values=" << j;
    return oss.str();
}
