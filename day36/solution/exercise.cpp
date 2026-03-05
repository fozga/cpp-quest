#include "exercise.hpp"
#include <unordered_map>
#include <vector>
#include <stdexcept>

struct StringInterner::Impl {
    std::unordered_map<std::string, StringInterner::id_type> to_id;
    std::vector<std::string> strings;
};

StringInterner::StringInterner() : impl_(std::make_unique<Impl>()) {}
StringInterner::~StringInterner() = default;
StringInterner::StringInterner(StringInterner&&) noexcept = default;
StringInterner& StringInterner::operator=(StringInterner&&) noexcept = default;

StringInterner::id_type StringInterner::intern(const std::string& s) {
    auto it = impl_->to_id.find(s);
    if (it != impl_->to_id.end()) return it->second;
    id_type id = impl_->strings.size();
    impl_->strings.push_back(s);
    impl_->to_id[s] = id;
    return id;
}

bool StringInterner::has_string(const std::string& s) const {
    return impl_->to_id.count(s) > 0;
}

bool StringInterner::has_id(id_type id) const {
    return id < impl_->strings.size();
}

const std::string& StringInterner::get_string(id_type id) const {
    if (id >= impl_->strings.size())
        throw std::out_of_range("StringInterner: invalid id");
    return impl_->strings[id];
}

StringInterner::id_type StringInterner::get_id(const std::string& s) const {
    auto it = impl_->to_id.find(s);
    if (it == impl_->to_id.end())
        throw std::out_of_range("StringInterner: string not found");
    return it->second;
}

std::size_t StringInterner::size() const noexcept {
    return impl_->strings.size();
}
