# C++20 Ranges

## Day Overview

The Ranges library, introduced in C++20, transforms the way we express
algorithms over sequences. Instead of manually writing loops or threading
iterator pairs through `<algorithm>` calls, you can compose lightweight,
lazy *views* with the pipe (`|`) operator — producing code that reads
almost like a description of what it does rather than how.

Key motivations:

- **Composition** — chain small operations (filter, transform, take) into
  pipelines without intermediate containers.
- **Lazy evaluation** — views compute elements on demand, avoiding
  unnecessary work or allocation.
- **Readability** — pipelines eliminate bookkeeping variables, making intent
  clearer than hand-written loops.

This builds directly on Day 16 (Custom Iterators): the standard range
concept requires `begin()` / `end()`, so every container that exposes
iterators already *is* a range. Views then add a composable algebra on top
of that foundation.

## Learning Objectives

- Explain the difference between a *range* (anything with `begin`/`end`)
  and a *view* (a lightweight, non-owning, lazy range).
- Use `std::views::filter` to select elements that satisfy a predicate.
- Use `std::views::transform` to project each element through a function.
- Use `std::views::take` and other standard view adaptors.
- Replace explicit loops with concise range pipelines using the pipe
  operator.
- Understand that views are non-owning and do not copy elements.
- Apply `std::ranges::count_if` and other range-based algorithms that
  accept ranges directly (no iterator pairs needed).

## Task Summary

Refactor loop-based text-processing logic into ranges pipelines. Given a
`std::vector<std::string>` of log lines you will:

1. **Filter** lines that contain `"ERROR"` and **uppercase** them —
   returning the results as a new vector.
2. **Count** the non-empty lines in the original vector.

## Implementation Requirements

| Function | Technique |
|---|---|
| `filter_and_uppercase_errors` | `std::views::filter` to keep lines containing `"ERROR"`, piped into `std::views::transform` to uppercase each line; collect results into a `std::vector<std::string>`. |
| `count_non_empty_lines` | `std::ranges::count_if` with a predicate that checks `!s.empty()`. |

## How to Run

From the repository root:

```bash
make day=17 test       # build and run tests
make day=17 asan       # AddressSanitizer build
make day=17 valgrind   # Valgrind memcheck
make day=17 clean      # remove binaries
```

## Suggested Workflow

1. Read `materials/notes.md` to understand ranges, views, and pipe syntax.
2. Study the function declarations in `exercise.hpp`.
3. Implement `filter_and_uppercase_errors` and `count_non_empty_lines` in
   `exercise.cpp` using range pipelines.
4. Run `make day=17 test` and iterate until all assertions pass.
5. Run the sanitizer and valgrind targets to verify memory safety.
