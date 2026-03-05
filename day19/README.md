# Type Traits

## Day Overview

Day 19 introduces type traits — compile-time utilities from `<type_traits>` that let you query and transform types without any runtime cost. Type traits are the foundation for advanced template techniques like SFINAE and C++20 Concepts, which we will explore in later days. Today we focus on understanding how traits work internally and writing a custom trait from scratch.

## Learning Objectives

- Understand what type traits are and how they enable compile-time type inspection
- Use standard traits from `<type_traits>` such as `std::is_integral`, `std::is_same`, and `std::is_floating_point`
- Recognise the `std::true_type` / `std::false_type` idiom and how `::value` exposes a compile-time boolean
- Write a custom type trait using a primary template and explicit or partial specializations
- Provide a `_v` variable template helper for cleaner syntax
- Use `static_assert` to verify trait results at compile time
- Understand how `std::basic_string<CharT, Traits, Alloc>` relates to `std::string`
- Distinguish between full specialization and partial specialization of class templates

## Task Summary

Implement an `is_std_string<T>` type trait that evaluates to `true` for `std::string` and any `std::basic_string<char, Traits, Alloc>` instantiation, and `false` for all other types including `std::wstring`, `const char*`, and arithmetic types.

## Implementation Requirements

| Component | Location | Description |
|---|---|---|
| `is_std_string<T>` | `exercise.hpp` | Primary template inheriting `std::false_type` |
| `is_std_string<basic_string<char, Traits, Alloc>>` | `exercise.hpp` | Partial specialization inheriting `std::true_type` |
| `is_std_string_v<T>` | `exercise.hpp` | `inline constexpr bool` variable template helper |
| `check_is_std_string<T>()` | `exercise.hpp` | `constexpr` function template returning the trait value |

## How to Run

```bash
make day=19 test      # compile and run tests
make day=19 asan      # run with AddressSanitizer
make day=19 valgrind  # run under Valgrind
make day=19 clean     # remove build artifacts
```

## Suggested Workflow

1. Read through `materials/notes.md` to understand how type traits and specialization work.
2. Study the `std::basic_string` class template signature so you know what to match.
3. Implement the primary template and partial specialization in `exercise.hpp`.
4. Add the `_v` helper and `check_is_std_string` function in the same header.
5. Run the tests and verify that all `static_assert` and runtime checks pass.
