# Project Layout

## Day Overview

As C++ projects grow beyond a handful of files, a clear **project layout** becomes essential for navigation, build configuration, and team collaboration. The standard convention separates code into directories by purpose — `include/` for public headers, `src/` for implementation files, and `tests/` for test drivers. Mapping logical modules to physical directories keeps dependencies visible and makes it straightforward to reason about what depends on what.

A well-organized layout means new contributors find code quickly, build systems stay manageable, and refactors stay contained within module boundaries.

## Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Recognize common C++ directory layouts (`include/`, `src/`, `tests/`). |
| 2 | Separate functionality into core, utility, and application modules. |
| 3 | Write module headers that expose minimal, focused interfaces. |
| 4 | Build a small driver that composes multiple modules together. |

## Task Summary

Implement three logical modules inside `exercise.hpp` / `exercise.cpp`:

* **Core** — `sum(v)` returns the sum of a `vector<int>`.
* **Util** — `join(v, separator)` produces a string of the vector's elements separated by the given character.
* **App** — `run_demo()` composes `sum` and `join` on the data `{1, 2, 3}` and returns a summary string: `"sum=6; values=1,2,3"`.

All three modules are declared in the header and defined in the source file. The test driver in `tests.cpp` exercises each module independently and then the composed result.

## How to Run

From the repository root:

```bash
make day=38 test       # build and run tests
make day=38 asan       # AddressSanitizer build
make day=38 valgrind   # Valgrind memcheck
make day=38 clean      # remove binaries
```

## Suggested Workflow

1. Read `materials/notes.md` to understand typical project layouts and module-to-directory mapping.
2. Study `exercise.hpp` — note how the three modules are grouped by comment sections.
3. Implement all functions in `exercise.cpp`.
4. Run `make day=38 test` and iterate until all assertions pass with zero warnings.
5. Review `materials/links.md` for deeper reading on project organization.
