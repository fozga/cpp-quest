# Putting It All Together

## Day Overview

This day brings together everything from the previous sessions — **refactoring**,
**unit testing**, **static analysis**, and **tooling** — into a single, cohesive
exercise. The goal is to design a small component that is clean enough to
refactor with confidence, tested enough to catch regressions, and structured so
that tools can verify its correctness. Code that meets all three criteria is code
that remains **safe to change**.

## Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Design a small, self-contained component with a clear public interface. |
| 2 | Write focused unit tests that cover normal operation **and** error paths. |
| 3 | Run the code under sanitizers (ASan, UBSan) to verify memory and behaviour safety. |
| 4 | Apply RAII, const-correctness, and proper error handling throughout. |

## Task Summary

Implement a `CounterService` class that manages a collection of **named
counters**.

| Method | Signature | Behaviour |
|--------|-----------|-----------|
| `add_counter` | `void add_counter(std::string name, int initial = 0)` | Create (or overwrite) a counter with the given name and initial value. |
| `increment` | `void increment(const std::string& name, int delta = 1)` | Add `delta` to an existing counter. Throws `std::out_of_range` if the counter does not exist. |
| `get` | `int get(const std::string& name) const` | Return the current value. Throws `std::out_of_range` if the counter does not exist. |
| `has` | `bool has(const std::string& name) const noexcept` | Return whether a counter with the given name exists. |
| `count` | `std::size_t count() const noexcept` | Return the total number of counters. |

## How to Run

From the repository root:

```bash
make day=35 test       # build and run tests
make day=35 asan       # AddressSanitizer build
make day=35 valgrind   # Valgrind memcheck
make day=35 clean      # remove binaries
```

## Suggested Workflow

1. Read `exercise.hpp` to understand the `CounterService` interface.
2. Study `tests.cpp` to see the expected behaviour — pay attention to error-path tests.
3. Implement each method in `exercise.cpp`, compiling and testing after each one.
4. Run under AddressSanitizer (`make day=35 asan`) to verify no memory errors.
5. Review: is every variable `const` where possible? Are names clear? Is there dead code?
