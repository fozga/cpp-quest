#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <variant>
#include <utility>
#include <stdexcept>

template <typename T, typename E>
class Expected {
public:
    Expected(const T& val) : data_(val), has_val_(true) {}
    Expected(T&& val) : data_(std::move(val)), has_val_(true) {}

    static Expected error(const E& err) {
        Expected e;
        e.data_ = err;
        e.has_val_ = false;
        return e;
    }
    static Expected error(E&& err) {
        Expected e;
        e.data_ = std::move(err);
        e.has_val_ = false;
        return e;
    }

    bool has_value() const noexcept { return has_val_; }
    explicit operator bool() const noexcept { return has_val_; }

    T& value() {
        if (!has_val_) throw std::logic_error("no value");
        return std::get<T>(data_);
    }
    const T& value() const {
        if (!has_val_) throw std::logic_error("no value");
        return std::get<T>(data_);
    }
    E& error() & {
        if (has_val_) throw std::logic_error("no error");
        return std::get<E>(data_);
    }
    const E& error() const& {
        if (has_val_) throw std::logic_error("no error");
        return std::get<E>(data_);
    }

private:
    Expected() : data_(E{}), has_val_(false) {}
    std::variant<T, E> data_;
    bool has_val_ = false;
};

enum class ParseError {
    TooShort,
    BadVersion,
    LengthMismatch
};

struct Packet {
    std::uint8_t version;
    std::uint16_t length;
    std::vector<std::uint8_t> payload;
};

std::string to_string(ParseError e);

Expected<Packet, ParseError> parse_packet(const std::vector<std::uint8_t>& data);
