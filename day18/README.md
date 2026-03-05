# Custom Views

## Day Overview

Welcome to **Day 18** of the 40-day C++ quest. Today you go beyond the
standard views provided by `<ranges>` and create your own reusable **custom
view adaptors**.

Building on the range concepts from Day 16 and the standard view adaptors from
Day 17, you will combine `std::views::filter`, range concepts, and lazy
evaluation over potentially infinite sequences to build a practical filtering
adaptor from scratch.

---

## Learning Objectives

By the end of this day you will be able to:

- Understand the structure of a custom view and what `view_interface` provides.
- Distinguish between a view type (holds state, provides begin/end) and a view
  adaptor (takes a range, returns a view).
- Write a template view adaptor constrained with `std::ranges::input_range`.
- Compose a custom view adaptor with standard views like `iota` and `take` in a
  pipeline.
- Use `std::views::filter` as the building block for a custom filtering adaptor.
- Apply lazy evaluation to generate elements on demand from an infinite sequence.
- Convert a lazy view into a concrete container when needed.

---

## Task Summary

Implement a `prime_filter` view adaptor that filters any integer range down to
only prime values. Use it together with `std::views::iota` and
`std::views::take` to build a `first_n_primes` utility that lazily generates
the first N prime numbers.

---

## Implementation Requirements

| Component | Location | Description |
|---|---|---|
| `is_prime` | `exercise.cpp` | Returns whether a `std::size_t` value is prime |
| `prime_filter` | `exercise.hpp` | Template view adaptor that filters a range to primes |
| `first_n_primes` | `exercise.cpp` | Returns a `std::vector<std::size_t>` of the first N primes |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=18 test

# Build with AddressSanitizer and run
make day=18 asan

# Build a normal binary then run under Valgrind
make day=18 valgrind

# Remove compiled artefacts
make day=18 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to review view concepts and custom adaptors.
2. **Implement** `is_prime` in `solution/exercise.cpp` — the tests will verify
   edge cases.
3. **Implement** `prime_filter` in `solution/exercise.hpp` using
   `std::views::filter`.
4. **Implement** `first_n_primes` in `solution/exercise.cpp` by composing
   `prime_filter` with `iota` and `take`.
5. **Compile** with `make day=18 test` — fix any compiler errors.
6. **Run** the tests and make every assertion pass.
7. **Run** under ASan (`make day=18 asan`) and Valgrind
   (`make day=18 valgrind`) to confirm clean execution.
8. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
9. **Browse** the links in `materials/links.md` for deeper exploration.
