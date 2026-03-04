# Custom Views

## Day Overview

Day 18 explores custom view types and view adaptors that go beyond the standard views provided by `<ranges>`. Building on the range concepts from Day 16 and the standard view adaptors from Day 17, we now create our own reusable view adaptors using `std::views::filter`, range concepts, and lazy evaluation over potentially infinite sequences.

## Learning Objectives

- Understand the structure of a custom view and what `view_interface` provides
- Distinguish between a view type (holds state, provides begin/end) and a view adaptor (takes a range, returns a view)
- Write a template view adaptor constrained with `std::ranges::input_range`
- Compose a custom view adaptor with standard views like `iota` and `take` in a pipeline
- Use `std::views::filter` as the building block for a custom filtering adaptor
- Apply lazy evaluation to generate elements on demand from an infinite sequence
- Convert a lazy view into a concrete container when needed

## Task Summary

Implement a `prime_filter` view adaptor that filters any integer range down to only prime values. Use it together with `std::views::iota` and `std::views::take` to build a `first_n_primes` utility that lazily generates the first N prime numbers.

## Implementation Requirements

| Component | Location | Description |
|---|---|---|
| `is_prime` | `exercise.cpp` | Returns whether a `std::size_t` value is prime |
| `prime_filter` | `exercise.hpp` | Template view adaptor that filters a range to primes |
| `first_n_primes` | `exercise.cpp` | Returns a `std::vector<std::size_t>` of the first N primes |

## How to Run

```bash
make day=18 test      # compile and run tests
make day=18 asan      # run with AddressSanitizer
make day=18 valgrind  # run under Valgrind
make day=18 clean     # remove build artifacts
```

## Suggested Workflow

1. Read through `materials/notes.md` to review view concepts and custom adaptors.
2. Implement `is_prime` in `exercise.cpp` — the tests will verify edge cases.
3. Implement `prime_filter` in `exercise.hpp` using `std::views::filter`.
4. Implement `first_n_primes` in `exercise.cpp` by composing `prime_filter` with `iota` and `take`.
5. Run the tests and fix any issues until all assertions pass.
