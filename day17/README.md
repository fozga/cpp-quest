# C++20 Ranges

## Day Overview

Welcome to **Day 17** of the 40-day C++ quest. Today you explore the C++20
**Ranges library** — a composable, lazy framework for expressing algorithms
over sequences.

The Ranges library transforms the way we express algorithms over sequences.
Instead of manually writing loops or threading iterator pairs through
`<algorithm>` calls, you can compose lightweight, lazy *views* with the pipe
(`|`) operator — producing code that reads almost like a description of what
it does rather than how.

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

---

## Learning Objectives

By the end of this day you will be able to:

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

---

## Task Summary

Refactor loop-based text-processing logic into ranges pipelines. Given a
`std::vector<std::string>` of log lines you will:

1. **Filter** lines that contain `"ERROR"` and **uppercase** them —
   returning the results as a new vector.
2. **Count** the non-empty lines in the original vector.

---

## Implementation Requirements

| Function | Technique |
|---|---|
| `filter_and_uppercase_errors` | `std::views::filter` to keep lines containing `"ERROR"`, piped into `std::views::transform` to uppercase each line; collect results into a `std::vector<std::string>`. |
| `count_non_empty_lines` | `std::ranges::count_if` with a predicate that checks `!s.empty()`. |

---

## How to Run

All commands are run from the repository root using the top-level Makefile:

```bash
# Build and run the test suite
make day=17 test

# Build with AddressSanitizer and run
make day=17 asan

# Build a normal binary then run under Valgrind
make day=17 valgrind

# Remove compiled artefacts
make day=17 clean
```

---

## Suggested Workflow

1. **Read** `materials/notes.md` to understand ranges, views, and pipe syntax.
2. **Study** the function declarations in `solution/exercise.hpp`.
3. **Implement** `filter_and_uppercase_errors` and `count_non_empty_lines` in
   `solution/exercise.cpp` using range pipelines.
4. **Compile** with `make day=17 test` — fix any compiler errors.
5. **Run** the tests and make every assertion pass.
6. **Run** under ASan (`make day=17 asan`) and Valgrind
   (`make day=17 valgrind`) to confirm clean execution.
7. **Answer** the self-check questions in `materials/notes.md` without
   looking at the notes.
8. **Browse** the links in `materials/links.md` for deeper exploration.
