# Unit Testing Mindset

## Day Overview

Unit tests are more than a safety net — they are a **design tool**. Writing tests
forces you to think about interfaces, edge cases, and expected behaviour before
(or alongside) production code. This day focuses on the **Unit Testing Mindset**:
structuring tests with the **Arrange-Act-Assert (AAA)** pattern and cultivating the
qualities that make tests genuinely useful — isolated, fast, deterministic, and
focused on a single behaviour.

## Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Structure every test with the **Arrange → Act → Assert** pattern. |
| 2 | Write focused tests that verify **one behaviour each**. |
| 3 | Recognise common **test smells** (flaky assertions, multi-concern tests, hidden dependencies). |

## Task Summary

Implement three `StringUtil` functions and verify them with focused, AAA-style
tests:

| Function | Signature |
|----------|-----------|
| `trim` | `std::string trim(std::string_view s)` |
| `to_upper` | `std::string to_upper(std::string_view s)` |
| `split_once` | `std::pair<std::string,std::string> split_once(std::string_view s, char delimiter)` |

## Implementation Requirements

### `trim`
- Remove leading and trailing whitespace (spaces, tabs, etc.).
- Use `static_cast<unsigned char>` when calling `std::isspace` to avoid UB.
- An all-whitespace or empty input returns `""`.

### `to_upper`
- Return a copy of the input with every character converted via `std::toupper`.
- Use `static_cast<unsigned char>` when calling `std::toupper`.
- An empty input returns `""`.

### `split_once`
- Split the string at the **first** occurrence of `delimiter`.
- If the delimiter is not found, return `{string(s), ""}`.
- If the delimiter is at position 0, the left part is `""`.
- Only split on the **first** match — everything after goes into the right part.

## How to Run

From the repository root:

```bash
make day=32 test       # build and run tests
make day=32 asan       # AddressSanitizer build
make day=32 valgrind   # Valgrind memcheck
make day=32 clean      # remove binaries
```

## Suggested Workflow

1. Read through `exercise.hpp` to understand the three function signatures.
2. Study `tests.cpp` to see the expected behaviour for each function.
3. Implement each function in `exercise.cpp`, compiling and testing after each one.
4. Look for edge cases — empty strings, all-whitespace, missing delimiters.
5. Review your tests: does each one follow AAA? Does each test check exactly one thing?
