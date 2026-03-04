# SFINAE to Concepts

## Day Overview

On Day 21 you used `std::enable_if` and SFINAE to build constrained `serialize`
overloads.  The technique works but produces verbose template signatures and
notoriously unhelpful error messages.  C++20 **Concepts** solve both problems:
you name the constraint once as a concept and apply it directly to the template
parameter.  This day walks through the mechanical translation from SFINAE to
concepts and highlights the readability and diagnostic improvements.

## Learning Objectives

After completing this day you will be able to:

- Translate an `enable_if`-based template constraint into an equivalent concept.
- Apply a concept as a constrained template parameter or via a `requires` clause.
- Appreciate the improved readability and compiler diagnostics that concepts provide.
- Implement concept-based `serialize_concept` function templates that mirror the SFINAE versions from Day 21.
- Distinguish between `IntegralSerializable` and `FloatingSerializable` concepts.

## Task Summary

Refactor Day 21's `serialize` overloads into concept-constrained
`serialize_concept` functions:

1. **`IntegralSerializable`** concept — satisfied when `std::is_integral_v<T>` is true.
2. **`FloatingSerializable`** concept — satisfied when `std::is_floating_point_v<T>` is true.
3. **`serialize_concept(T)`** — integral overload, returns `"int:<value>"`.
4. **`serialize_concept(T)`** — floating-point overload, returns `"float:<value>"`.
5. **`serialize_concept(const std::string&)`** — non-template overload, returns `"str:<value>"`.

## Implementation Requirements

```cpp
template <typename T>
concept IntegralSerializable = std::is_integral_v<T>;

template <typename T>
concept FloatingSerializable = std::is_floating_point_v<T>;

template <IntegralSerializable T>
std::string serialize_concept(T value);

template <FloatingSerializable T>
std::string serialize_concept(T value);

std::string serialize_concept(const std::string& value);
```

- Template overloads must be defined in the header (`exercise.hpp`).
- The `std::string` overload is a regular function declared in the header and defined in `exercise.cpp`.

## How to Run

From the repository root:

```bash
make day=23 test       # build and run tests
make day=23 asan       # AddressSanitizer build
make day=23 valgrind   # Valgrind memcheck
make day=23 clean      # remove binaries
```

## Suggested Workflow

1. Read `materials/notes.md` for a side-by-side comparison of SFINAE and concepts.
2. Open `solution/exercise.hpp` and compare the concept-constrained overloads with Day 21's `enable_if` versions.
3. Run the tests to confirm all overloads resolve correctly.
4. Try calling `serialize_concept` with a type that satisfies neither concept (e.g., `std::vector<int>`) and observe the diagnostic.
5. Review `materials/links.md` for deeper reading.
