#include "exercise.hpp"

std::string to_string(ParseError e) {
    switch (e) {
        case ParseError::TooShort:       return "TooShort";
        case ParseError::BadVersion:     return "BadVersion";
        case ParseError::LengthMismatch: return "LengthMismatch";
    }
    return "Unknown";
}

Expected<Packet, ParseError> parse_packet(const std::vector<std::uint8_t>& data) {
    if (data.size() < 3)
        return Expected<Packet, ParseError>::error(ParseError::TooShort);

    std::uint8_t version = data[0];
    if (version != 1)
        return Expected<Packet, ParseError>::error(ParseError::BadVersion);

    std::uint16_t length = (static_cast<std::uint16_t>(data[1]) << 8) | data[2];
    if (data.size() - 3 != length)
        return Expected<Packet, ParseError>::error(ParseError::LengthMismatch);

    return Packet{version, length, {data.begin() + 3, data.end()}};
}
