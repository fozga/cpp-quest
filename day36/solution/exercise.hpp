#pragma once
#include <string>
#include <cstddef>
#include <memory>

class StringInterner {
public:
    using id_type = std::size_t;

    StringInterner();
    ~StringInterner();

    StringInterner(const StringInterner&) = delete;
    StringInterner& operator=(const StringInterner&) = delete;
    StringInterner(StringInterner&&) noexcept;
    StringInterner& operator=(StringInterner&&) noexcept;

    id_type intern(const std::string& s);
    bool has_string(const std::string& s) const;
    bool has_id(id_type id) const;
    const std::string& get_string(id_type id) const;
    id_type get_id(const std::string& s) const;
    std::size_t size() const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
