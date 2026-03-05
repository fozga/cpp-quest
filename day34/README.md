# Thread Pool Foundation

## Day Overview

Static analysis tools examine your source code **without running it**, catching
bugs, style issues, and potential undefined behaviour that compilers miss. Tools
like **clang-tidy** and **cppcheck** enforce best practices — const-correctness,
proper ownership via smart pointers, dead-code elimination — that overlap heavily
with the RAII and resource-management skills from earlier days. The punchline:
writing code that satisfies static analysis is the same as writing **clear,
maintainable code**.

## Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Understand common static-analysis warnings and what they protect against. |
| 2 | Apply **const-correctness** to every variable and parameter that should not change. |
| 3 | Use clear **ownership** semantics (value types, `std::unique_ptr`, references). |
| 4 | Identify and remove **dead code** — unused variables, unreachable branches, redundant includes. |
| 5 | Choose descriptive, unambiguous **names** for functions and variables. |

## Task Summary

Implement two utility functions with clean, analysis-friendly style:

| Function | Signature |
|----------|-----------|
| `format_timestamp` | `std::string format_timestamp(std::chrono::system_clock::time_point tp)` |
| `format_duration_ms` | `std::string format_duration_ms(std::chrono::milliseconds ms)` |

### `format_timestamp`
- Convert a `system_clock::time_point` to a string in `"YYYY-MM-DD HH:MM"` format (16 characters).
- Use `std::localtime` and `std::put_time`.
- Mark intermediate values `const` wherever possible.

### `format_duration_ms`
- Format a duration as `"<seconds>.<milliseconds>s"`, e.g. `1500ms` → `"1.500s"`.
- The milliseconds portion is always three digits, zero-padded.
- `0ms` → `"0.000s"`.

## How to Run

```bash
cd day34/solution
make          # compile and run tests
make asan     # compile with AddressSanitizer and run
make valgrind # run under Valgrind
make clean    # remove binaries
```

## Suggested Workflow

1. Read `exercise.hpp` to understand the two function signatures.
2. Study `tests.cpp` to see the expected output format and edge cases.
3. Implement each function in `exercise.cpp`, compiling after each one.
4. Run the tests — fix any assertion failures.
5. Review your code: is every possible variable `const`? Are names descriptive? Is there any dead code?
