#include "exercise.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> lines = {
        "port=8080",
        "use_tls=true",
        "device_name=board01",
        "bad_int=abc",
        "flag_off=false",
        "flag_zero=0",
    };

    Config cfg = make_config_from_lines(lines);

    // get_int: valid integer
    auto port = get_int(cfg, "port");
    assert(port.has_value());
    assert(port.value() == 8080);

    // get_bool: "true"
    auto tls = get_bool(cfg, "use_tls");
    assert(tls.has_value());
    assert(tls.value() == true);

    // get_string: valid key
    auto name = get_string(cfg, "device_name");
    assert(name.has_value());
    assert(name.value() == "board01");

    // get_string: missing key returns empty optional
    assert(!get_string(cfg, "missing_key").has_value());

    // get_int: unparseable integer returns empty optional
    assert(!get_int(cfg, "bad_int").has_value());

    // get_bool: "false" parses to false
    auto foff = get_bool(cfg, "flag_off");
    assert(foff.has_value());
    assert(foff.value() == false);

    // get_bool: "0" parses to false
    auto fzero = get_bool(cfg, "flag_zero");
    assert(fzero.has_value());
    assert(fzero.value() == false);

    std::cout << "[day12] All tests passed.\n";
    return 0;
}
