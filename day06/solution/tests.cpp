#include "exercise.hpp"

#include <cassert>
#include <iostream>
#include <string>

int main() {
    {
        auto q = make_sample_query();
        std::string s = q.str();
        assert(s.find("users") != std::string::npos);
        assert(s.find("id") != std::string::npos);
        assert(s.find("name") != std::string::npos);
        assert(s.find("active") != std::string::npos);
        assert(s.find("50") != std::string::npos);
        std::cout << "[pass] make_sample_query: " << s << "\n";
    }

    {
        auto q2 = QueryBuilder::from_table("logs");
        q2.select("id").where("level = 'error'").limit(100);
        std::string s2 = q2.str();
        assert(s2.find("logs") != std::string::npos);
        assert(s2.find("id") != std::string::npos);
        assert(s2.find("error") != std::string::npos);
        assert(s2.find("100") != std::string::npos);
        std::cout << "[pass] from_table + chaining: " << s2 << "\n";

        auto q3 = std::move(q2);
        std::string s3 = q3.str();
        assert(s3.find("logs") != std::string::npos);
        assert(s3.find("id") != std::string::npos);
        std::cout << "[pass] move construction: " << s3 << "\n";

        std::string s2_after = q2.str();
        std::cout << "[info] moved-from state: \"" << s2_after << "\"\n";
    }

    {
        auto q4 = QueryBuilder::from_table("products");
        std::string s4 = q4.str();
        assert(s4.find("SELECT *") != std::string::npos);
        assert(s4.find("products") != std::string::npos);
        std::cout << "[pass] no selects defaults to *: " << s4 << "\n";
    }

    std::cout << "\nAll day 06 tests passed.\n";
    return 0;
}
