# Exception Removal

## Day Overview

Traditional C++ error handling relies on exceptions — `throw`, `try`/`catch`, and stack
unwinding. While powerful, exceptions carry hidden costs: stack unwinding is expensive,
control flow is invisible at the call site, and many low-latency, embedded, or real-time
systems disable exceptions entirely (`-fno-exceptions`).

C++23 introduces `std::expected<T, E>` as a vocabulary type that encodes success-or-failure
directly in a function's return type. The caller sees immediately that the function can
fail, what error type it produces, and must explicitly decide how to handle each path.

In this exercise you will implement a minimal `Expected<T, E>` template (since
`std::expected` requires C++23 library support) and use it to build a network-packet
parser that never throws.

## Learning Objectives

- Understand what `Expected<T, E>` represents: a discriminated union of a value or an error.
- Check whether an `Expected` holds a value or an error.
- Access the contained value or error safely.
- Refactor throw-based code into return-based error handling using `Expected`.
- Appreciate how error types appear in function signatures, making failure paths explicit.
- Implement a custom `Expected` template using `std::variant`.
- Understand the concept of monadic composition (`transform`, `and_then`) for chaining
  fallible operations.
- Compare when exceptions vs value-based error handling is the better choice.

## Task Summary

Implement a network packet parser that returns `Expected<Packet, ParseError>` instead of
throwing exceptions. The parser reads a raw byte buffer and validates a version byte, a
big-endian length field, and the payload.

## Implementation Requirements

| Component | Description |
|-----------|-------------|
| `ParseError` enum class | `TooShort`, `BadVersion`, `LengthMismatch` |
| `Expected<T, E>` template | Minimal implementation in the header using `std::variant` |
| `Packet` struct | `version` (uint8), `length` (uint16), `payload` (vector of uint8) |
| `parse_packet` function | Validates and parses raw bytes, returns `Expected<Packet, ParseError>` |
| `to_string(ParseError)` | Converts error enum to a human-readable string |

## How to Run

From the repository root:

```bash
make day=14 test      # build and run tests
make day=14 asan      # build with AddressSanitizer and run
make day=14 valgrind  # build normally, then run under Valgrind
make day=14 clean     # remove built binaries
```

## Suggested Workflow

1. Read `materials/notes.md` to understand exceptions vs return-based errors.
2. Study the `Expected<T, E>` template in `exercise.hpp`.
3. Implement `to_string` and `parse_packet` in `exercise.cpp`.
4. Run the tests and iterate until all assertions pass.
5. Try extending the parser with additional validation steps.
