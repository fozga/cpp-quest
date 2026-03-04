# SFINAE Constraints

## Day Overview

SFINAE — *Substitution Failure Is Not An Error* — is a foundational C++ mechanism
that silently removes a function-template overload from the candidate set when
substituting template arguments produces an invalid type.  Combined with
`std::enable_if` and the type traits you explored on Day 19, SFINAE lets you
write constrained generic code that selects the right overload at compile time
without runtime branching.

## Learning Objectives

After completing this day you will be able to:

- Define what SFINAE means and explain the substitution-failure rule.
- Use `std::enable_if` / `std::enable_if_t` to conditionally enable a template.
- Apply type-trait predicates (`std::is_integral_v`, `std::is_floating_point_v`, etc.) inside `enable_if` conditions.
- Implement mutually exclusive constrained overloads that the compiler can resolve unambiguously.
- Recognize and interpret the verbose error messages that arise from failed SFINAE constraints.
- Explain why SFINAE motivates the cleaner Concepts syntax introduced in C++20.
- Place template definitions in headers so the compiler can instantiate them in every translation unit.

## Task Summary

Refactor a generic `serialize` function template into **two** constrained
overloads using `std::enable_if_t`:

1. **Integral overload** — enabled when `std::is_integral_v<T>` is `true`.
   Returns a string of the form `"int:<value>"`.
2. **Non-integral overload** — enabled when `std::is_integral_v<T>` is `false`.
   Returns a string of the form `"other:<value>"`.

Both overloads return `std::string` and must be defined entirely in the header
(`exercise.hpp`).

## Implementation Requirements

```cpp
// Integral overload
template <typename T>
std::enable_if_t<std::is_integral_v<T>, std::string>
serialize(T value);

// Non-integral overload
template <typename T>
std::enable_if_t<!std::is_integral_v<T>, std::string>
serialize(const T& value);
```

- Use `<sstream>` (`std::ostringstream`) to build the output string.
- `exercise.cpp` only includes the header — all template logic lives in
  `exercise.hpp`.

## How to Run

From the repository root:

```bash
make day=21 test       # build and run tests
make day=21 asan       # AddressSanitizer build
make day=21 valgrind   # Valgrind memcheck
make day=21 clean      # remove binaries
```

## Suggested Workflow

1. Read `materials/notes.md` for the theory behind SFINAE and `enable_if`.
2. Open `solution/exercise.hpp` and study the two overloads.
3. Try modifying a constraint (e.g., remove the `!` on the non-integral overload) and observe the compiler error.
4. Run the tests to confirm both overloads resolve correctly.
5. Review `materials/links.md` for deeper reading.
