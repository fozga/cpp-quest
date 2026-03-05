#include "exercise.hpp"

QueryBuilder::QueryBuilder(QueryBuilder&& other) noexcept
    : selects_(std::move(other.selects_)),
      table_(std::move(other.table_)),
      conditions_(std::move(other.conditions_)),
      limit_(other.limit_) {
    other.limit_ = -1;
}

QueryBuilder& QueryBuilder::operator=(QueryBuilder&& other) noexcept {
    if (this != &other) {
        selects_ = std::move(other.selects_);
        table_ = std::move(other.table_);
        conditions_ = std::move(other.conditions_);
        limit_ = other.limit_;
        other.limit_ = -1;
    }
    return *this;
}

QueryBuilder QueryBuilder::from_table(std::string table) {
    QueryBuilder qb;
    qb.table_ = std::move(table);
    return qb;
}

QueryBuilder& QueryBuilder::select(std::string column) {
    selects_.push_back(std::move(column));
    return *this;
}

QueryBuilder& QueryBuilder::where(std::string condition) {
    conditions_.push_back(std::move(condition));
    return *this;
}

QueryBuilder& QueryBuilder::limit(int n) {
    limit_ = n;
    return *this;
}

std::string QueryBuilder::str() const {
    std::ostringstream out;

    out << "SELECT ";
    if (selects_.empty()) {
        out << "*";
    } else {
        for (std::size_t i = 0; i < selects_.size(); ++i) {
            if (i > 0) out << ", ";
            out << selects_[i];
        }
    }

    out << " FROM " << table_;

    if (!conditions_.empty()) {
        out << " WHERE ";
        for (std::size_t i = 0; i < conditions_.size(); ++i) {
            if (i > 0) out << " AND ";
            out << conditions_[i];
        }
    }

    if (limit_ >= 0) {
        out << " LIMIT " << limit_;
    }

    return out.str();
}

QueryBuilder make_sample_query() {
    auto q = QueryBuilder::from_table("users");
    q.select("id").select("name").where("active = true").limit(50);
    return q;
}
