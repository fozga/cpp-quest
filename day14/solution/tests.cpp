#include "exercise.hpp"
#include <cassert>
#include <iostream>

int main() {
    // Valid packet: version=1, length=3 (big-endian: 0x00 0x03), 3 payload bytes
    {
        std::vector<std::uint8_t> buf = {1, 0, 3, 0xAA, 0xBB, 0xCC};
        auto r = parse_packet(buf);
        assert(r.has_value());
        assert(r.value().version == 1);
        assert(r.value().length == 3);
        assert(r.value().payload.size() == 3);
        assert(r.value().payload[0] == 0xAA);
        assert(r.value().payload[1] == 0xBB);
        assert(r.value().payload[2] == 0xCC);
        std::cout << "[PASS] valid packet\n";
    }

    // Too short
    {
        std::vector<std::uint8_t> buf = {1};
        auto r = parse_packet(buf);
        assert(!r.has_value());
        assert(r.error() == ParseError::TooShort);
        std::cout << "[PASS] too short\n";
    }

    // Bad version
    {
        std::vector<std::uint8_t> buf = {2, 0, 0};
        auto r = parse_packet(buf);
        assert(!r.has_value());
        assert(r.error() == ParseError::BadVersion);
        std::cout << "[PASS] bad version\n";
    }

    // Length mismatch (header says 5 bytes but only 1 payload byte present)
    {
        std::vector<std::uint8_t> buf = {1, 0, 5, 0xAA};
        auto r = parse_packet(buf);
        assert(!r.has_value());
        assert(r.error() == ParseError::LengthMismatch);
        std::cout << "[PASS] length mismatch\n";
    }

    std::cout << "\nAll day-14 tests passed.\n";
    return 0;
}
