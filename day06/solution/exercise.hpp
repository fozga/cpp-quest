#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <utility>

class QueryBuilder {
    std::vector<std::string> selects_;
    std::string table_;
    std::vector<std::string> conditions_;
    int limit_ = -1;

public:
    QueryBuilder() = default;
    ~QueryBuilder() = default;

    QueryBuilder(const QueryBuilder&) = delete;
    QueryBuilder& operator=(const QueryBuilder&) = delete;

    QueryBuilder(QueryBuilder&& other) noexcept;
    QueryBuilder& operator=(QueryBuilder&& other) noexcept;

    static QueryBuilder from_table(std::string table);

    QueryBuilder& select(std::string column);
    QueryBuilder& where(std::string condition);
    QueryBuilder& limit(int n);

    std::string str() const;
};

QueryBuilder make_sample_query();
